/**
* @file Actor.hpp
* @brief The Actor class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-08-28
*/


#ifndef ANTFARM_ACTOR_HPP
#define ANTFARM_ACTOR_HPP

#include "scene/2d/physics_body_2d.h"

class ActorSignals
{
  public:
  static ActorSignals * singleton();

  StringName died_signal = StaticCString::create("died");
  StringName damaged_signal = StaticCString::create("damaged");

  private:
  static ActorSignals * m_singleton;
};

class Actor : public KinematicBody2D
{
  GDCLASS(Actor, KinematicBody2D);

  public:
    Actor();

    int get_hitpoints() const;

    int set_hitpoints(
        int p_hitpoints);

    int get_current_hitpoints() const;

    void damage(
        int p_points,
        Vector2 p_position);

    void heal(
        int p_points);

    void die();

    void face_left();

    void face_right();

    void stop();

    Vector2 linear_velocity() const;

    void apply_impulse(
        Vector2 p_impulse);

  protected:
    static void _bind_methods();

    void _notification(
        int const notification);

  private:
    int m_hitpoints;
    int m_current_hitpoints;
    int m_biomass;
    bool m_facing_right;
    Vector2 m_velocity;
};

#endif

