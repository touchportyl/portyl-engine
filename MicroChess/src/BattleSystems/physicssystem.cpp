#include "physicssystem.h"
#include "Components/physics.h"
#include "Components/rendering.h"	//position, scale, rotate still over here

#include <FlexEngine.h>

#include <vector>
#include <cmath>
#include <algorithm>


namespace ChronoShift
{
	struct Collision
	{
		FlexECS::Entity a;
		FlexECS::Entity b;
	};
	std::vector<Collision> g_collisions {};



	void UpdatePositions() 
	{
		float dt = FlexEngine::Application::GetCurrentWindow()->GetDeltaTime();
		for (auto& entity : FlexECS::Scene::GetActiveScene()->View<Position, Rigidbody>())
		{
			auto& velocity = entity.GetComponent<Rigidbody>()->velocity;
			auto& position = entity.GetComponent<Position>()->position;

			position.x += velocity.x * dt;
			position.y += velocity.y * dt;
		}
	}
	
	void UpdateBounds()
	{
		for (auto& entity : FlexECS::Scene::GetActiveScene()->View<Position, Scale, Rigidbody, BoundingBox2D>()) 
		{
			auto& position = entity.GetComponent<Position>()->position;
			auto& scale = entity.GetComponent<Scale>()->scale;
			auto& size = entity.GetComponent<BoundingBox2D>()->size;
			entity.GetComponent<BoundingBox2D>()->max.x = position.x + scale.x / 2 * size.x;
			entity.GetComponent<BoundingBox2D>()->max.y = position.y + scale.y / 2 * size.y;
			entity.GetComponent<BoundingBox2D>()->min.x = position.x - scale.x / 2 * size.x;
			entity.GetComponent<BoundingBox2D>()->min.y = position.y - scale.y / 2 * size.y;
		}

	}

	void FindCollisions() 
	{
		g_collisions.clear();
		
		for (auto& entity_a : FlexECS::Scene::GetActiveScene()->View<Position, Scale, Rigidbody, BoundingBox2D>())
		{
			if(entity_a.GetComponent<Rigidbody>()->is_static) continue;
			//construct aabb
			auto& max_a = entity_a.GetComponent<BoundingBox2D>()->max;
			auto& min_a = entity_a.GetComponent<BoundingBox2D>()->min;
			
			for (auto& entity_b : FlexECS::Scene::GetActiveScene()->View<Position, Scale, Rigidbody, BoundingBox2D>()) 
			{
				if (entity_a == entity_b) continue;

				auto& max_b = entity_b.GetComponent<BoundingBox2D>()->max;
				auto& min_b = entity_b.GetComponent<BoundingBox2D>()->min;

				//AABB check
				if (max_a.x < min_b.x || max_a.y < min_b.y || min_a.x > max_b.x || min_a.y > max_b.y) continue;
				else g_collisions.push_back({ entity_a, entity_b });
			}
		}

	}

	void ResolveCollisions() 
	{
		float dt = FlexEngine::Application::GetCurrentWindow()->GetDeltaTime();
		for (Collision collision : g_collisions)
		{
			auto& a_velocity = collision.a.GetComponent<Rigidbody>()->velocity;
			auto& a_position = collision.a.GetComponent<Position>()->position;
			auto& b_velocity = collision.b.GetComponent<Rigidbody>()->velocity;
			auto& b_position = collision.b.GetComponent<Position>()->position;

			auto& a_max = collision.a.GetComponent<BoundingBox2D>()->max;
			auto& a_min = collision.a.GetComponent<BoundingBox2D>()->min;
			auto& b_max = collision.b.GetComponent<BoundingBox2D>()->max;
			auto& b_min = collision.b.GetComponent<BoundingBox2D>()->min;

			//Check if already resolved
			if (a_max.x < b_min.x || a_max.y < b_min.y || a_min.x > b_max.x || a_min.y > b_max.y) continue;


			Vector2 normal = a_position - b_position;

			const float a_half_width = (a_max.x - a_min.x) / 2.0f;
			const float b_half_width = (b_max.x - b_min.x) / 2.0f;
			const float a_half_height = (a_max.y - a_min.y) / 2.0f;
			const float b_half_height = (b_max.y - b_min.y) / 2.0f;

			const float x_penetration = a_half_width + b_half_width - std::abs(normal.x);
			const float y_penetration = a_half_height + b_half_height - std::abs(normal.y);

			//find shortest collision side 
			const float left = a_max.x - b_min.x;
			const float right = b_max.x - a_min.x;
			const float up = a_max.y - b_min.y;
			const float down = b_max.y - a_min.y;
			const float largest = std::min({ left, right, up, down });

			if (!(collision.a.GetComponent<Rigidbody>()->is_static))
			{
				if (largest == left) {
					a_position.x -= x_penetration;
				}
				else if (largest == right) {
					a_position.x += x_penetration;
				}
				else if (largest == up) {
					a_position.y -= y_penetration;
				}
				else if (largest == down) {
					a_position.y += y_penetration;
				}
			}
			
			if (!(collision.b.GetComponent<Rigidbody>()->is_static))
			{
				if (largest == left) {
					b_position.x += x_penetration;
				}
				else if (largest == right) {
					b_position.x -= x_penetration;
				}
				else if (largest == up) {
					b_position.y += y_penetration;
				}
				else if (largest == down) {
					std::cout << "pls" << b_position.y;
					b_position.y -= y_penetration;
					std::cout << "     new pos " << collision.b.GetComponent<Position>()->position.y << std::endl;
				}
			}
		}
	}


	void UpdatePhysicsSystem()
	{
		UpdatePositions();
		UpdateBounds();
		FindCollisions();
		ResolveCollisions();
	}

}