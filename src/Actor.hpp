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

    void set_falling(
        bool fall);

    bool is_falling() const;

    int get_biomass() const;

    int set_biomass(
        int p_biomass);

    float get_jump_speed() const;
        
    void set_jump_speed(
        float p_speed);

    float get_run_speed() const;
        
    void set_run_speed(
        float p_speed);

    void jump();

    void set_movement(
        int p_move);

    int get_movement() const;

  protected:
    static void _bind_methods();

    void _notification(
        int const notification);

  private:
    int m_hitpoints;
    int m_current_hitpoints;
    int m_biomass;
    float m_jump_speed;
    float m_run_speed;
    bool m_facing_right;
    bool m_falling;
    bool m_jump_triggered;
    int m_horizontal_move;
    Vector2 m_velocity;
};

#endif

