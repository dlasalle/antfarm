/**
 * @file Actor.cpp
 * @brief Implementation of Actor class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2018
 * @version 1
 * @date 2018-08-28
 */

#include "Actor.hpp"

#include "string_db.h"
#include "servers/physics_2d_server.h"

#include <algorithm>


/******************************************************************************
* PUBLIC STATIC METHODS *******************************************************
******************************************************************************/

ActorSignals * ActorSignals::m_singleton = nullptr;

ActorSignals * ActorSignals::singleton()
{
  if (m_singleton == nullptr) {
    m_singleton = new ActorSignals;
  }

  return m_singleton;
}
 
/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/

Actor::Actor() :
  m_hitpoints(1),
  m_current_hitpoints(1),
  m_biomass(1),
  m_facing_right(false),
  m_jump_speed(100.0),
  m_run_speed(75.0),
  m_falling(true),
  m_jump_triggered(false),
  m_horizontal_move(0),
  m_velocity(0,0)
{
  // do nothing
}

/******************************************************************************
* PUBLIC METHODS **************************************************************
******************************************************************************/

int Actor::get_hitpoints() const
{
  return m_hitpoints;
}

int Actor::set_hitpoints(
    int const p_hitpoints)
{
  m_hitpoints = p_hitpoints;
  m_current_hitpoints = m_hitpoints;
}

int Actor::get_current_hitpoints() const
{
  return m_current_hitpoints;
}

void Actor::damage(
    int const p_points,
    Vector2 p_position)
{
  int const initial_hitpoints = get_current_hitpoints();
  m_current_hitpoints = std::max(0, m_current_hitpoints - p_points);

  emit_signal(ActorSignals::singleton()->damaged_signal, p_points, p_position);

  if (initial_hitpoints > 0 && m_current_hitpoints == 0) {
    die();
  }
}

void Actor::heal(
    int const p_points)
{
  m_current_hitpoints = std::min(m_hitpoints, m_current_hitpoints + p_points);
}

void Actor::die()
{
  m_current_hitpoints = 0;
  emit_signal(ActorSignals::singleton()->died_signal);
  queue_delete();
}

void Actor::face_left()
{
  if (m_facing_right) {
    m_facing_right = false;
    this->apply_scale(Size2(-1, 1));
  }
}

void Actor::face_right()
{
  if (!m_facing_right) {
    m_facing_right = true;
    this->apply_scale(Size2(-1, 1));
  }
}

void Actor::stop()
{
  m_velocity = Vector2(0,0);
}

Vector2 Actor::linear_velocity() const
{
  return m_velocity;
}

void Actor::apply_impulse(
    Vector2 const p_impulse)
{
  m_velocity += p_impulse;
}


void Actor::set_falling(
    bool const p_fall)
{
  m_falling = p_fall;
}

bool Actor::is_falling() const
{
  return m_falling;
}

float Actor::get_jump_speed() const
{
  return m_jump_speed;
}
    
void Actor::set_jump_speed(
    float const p_speed)
{
  m_jump_speed = p_speed;
}

float Actor::get_run_speed() const
{
  return m_run_speed;
}
    
void Actor::set_run_speed(
    float const p_speed)
{
  m_run_speed = p_speed;
}

void Actor::jump()
{
  m_jump_triggered = true;
}

void Actor::set_movement(
    int p_move)
{
  m_horizontal_move = p_move;
}

int Actor::get_movement() const
{
  return m_horizontal_move;
}



/******************************************************************************
* PROTECTED STATIC METHODS ****************************************************
******************************************************************************/

void Actor::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_hitpoints"), &Actor::get_hitpoints);
	ClassDB::bind_method(D_METHOD("set_hitpoints", "hitpoints"), \
      &Actor::set_hitpoints);

	ClassDB::bind_method(D_METHOD("get_current_hitpoints"), \
      &Actor::get_current_hitpoints);

	ClassDB::bind_method(D_METHOD("damage", "points", "position"), &Actor::damage);
	ClassDB::bind_method(D_METHOD("heal", "points"), &Actor::heal);

	ClassDB::bind_method(D_METHOD("die"), &Actor::die);

	ClassDB::bind_method(D_METHOD("face_left"), &Actor::face_left);
	ClassDB::bind_method(D_METHOD("face_right"), &Actor::face_right);

	ClassDB::bind_method(D_METHOD("stop"), &Actor::stop);

	ClassDB::bind_method(D_METHOD("linear_velocity"), &Actor::linear_velocity);
	ClassDB::bind_method(D_METHOD("apply_impulse", "impulse"), \
      &Actor::apply_impulse);

	ClassDB::bind_method(D_METHOD("set_falling", "fall"), \
      &Actor::set_falling);
	ClassDB::bind_method(D_METHOD("is_falling"), &Actor::is_falling);

  ClassDB::bind_method(D_METHOD("set_jump_speed", "speed"), \
      &Actor::set_jump_speed);
  ClassDB::bind_method(D_METHOD("get_jump_speed"), &Actor::get_jump_speed);

  ClassDB::bind_method(D_METHOD("set_run_speed", "speed"), \
      &Actor::set_run_speed);
  ClassDB::bind_method(D_METHOD("get_run_speed"), &Actor::get_run_speed);

  ClassDB::bind_method(D_METHOD("jump"), &Actor::jump);

  ClassDB::bind_method(D_METHOD("set_movement", "move"), &Actor::set_movement);
  ClassDB::bind_method(D_METHOD("get_movement"), &Actor::get_movement);

	ADD_SIGNAL(MethodInfo("died"));
	ADD_SIGNAL(MethodInfo("damaged", PropertyInfo(Variant::INT, "points"), \
      PropertyInfo(Variant::VECTOR2, "position")));


	ADD_PROPERTY(PropertyInfo(Variant::INT, "hitpoints", PROPERTY_HINT_RANGE, \
      "1,100000"), "set_hitpoints", "get_hitpoints");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "biomass", PROPERTY_HINT_RANGE, \
      "0,100000"), "set_biomass", "get_biomass");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "jump_speed", PROPERTY_HINT_RANGE, \
      "0,1000.0"), "set_jump_speed", "get_jump_speed");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "run_speed", PROPERTY_HINT_RANGE, \
      "0,1000.0"), "set_run_speed", "get_run_speed");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "falling"), "set_falling",
      "is_falling");
}


/******************************************************************************
* PROTECTED METHODS ***********************************************************
******************************************************************************/

void Actor::_notification(
    int const p_notification)
{
  switch (p_notification)
  {
    case NOTIFICATION_PHYSICS_PROCESS: {
      if (m_jump_triggered) {
        if (is_on_floor()) {
          m_velocity += Vector2(0, -m_jump_speed);
        }
        m_jump_triggered = false;
      }

      if (m_falling) {
        Physics2DDirectBodyState * const state = \
            Physics2DServer::get_singleton()->body_get_direct_state(get_rid());
        Vector2 const gravity = state->get_total_gravity();
        m_velocity += gravity*get_physics_process_delta_time();
      }

      if (is_on_floor()) {
        m_velocity.x = m_run_speed*m_horizontal_move;
      }

      if (m_velocity.length_squared() > 0) {
        // move actor
        m_velocity = move_and_slide(m_velocity, Vector2(0, -1.0), false);
      }
    }
    break;
  }
}
