#include "Renderer/sprite2d.h"

#include "Components/rendering.h"

#define PostProcessing 0
namespace ChronoShift
{
    //If u want this function to be callable, tell wei jie, otherwise not supposed to be called outside of sprite2D
    static void UpdateTransformationMatrix(FlexECS::Entity& currEntity, const Matrix4x4 parent_entity_matrix = Matrix4x4::Identity)
    {
        auto& local_transform = currEntity.GetComponent<Transform>()->transform;
        //if (!currEntity.GetComponent<Transform>()->is_dirty) return; //SOMETHING WRONG WITH IS_DIRTY

        auto& local_position = currEntity.GetComponent<Position>()->position;
        auto& local_scale = currEntity.GetComponent<Scale>()->scale;
        auto& local_rotation = currEntity.GetComponent<Rotation>()->rotation;

        //Alignment of sprite
        static Vector2 sprite_alignment = Vector2::Zero;
        switch (currEntity.GetComponent<Sprite>()->alignment)
        {
        case Renderer2DProps::Alignment::Alignment_TopLeft:
            sprite_alignment = Vector2(local_scale * 0.5f);
            break;
        case Renderer2DProps::Alignment::Alignment_Center:
        default:
            sprite_alignment = Vector2::Zero;
            break;
        }

        // calculate the transform
        Matrix4x4 translation_matrix = Matrix4x4::Translate(Matrix4x4::Identity, Vector3(-(local_position.x + sprite_alignment.x), local_position.y + sprite_alignment.y, 0.0f));
        Matrix4x4 rotation_matrix = Quaternion::FromEulerAnglesDeg(local_rotation).ToRotationMatrix();
        Matrix4x4 scale_matrix = Matrix4x4::Scale(Matrix4x4::Identity, local_scale);

        Matrix4x4 curr_entity_matrix = translation_matrix * rotation_matrix * scale_matrix;
        local_transform = parent_entity_matrix * curr_entity_matrix;
    }

    void UpdateSprite2DMatrix()
    {
        //Uncomment log for debug testing
        static std::vector<FlexECS::Entity*> t_entitystack;
        t_entitystack.clear();  // Clear stack at the beginning of each update
        // Unordered set to track processed entities (avoid redundant updates)
        static std::unordered_set<FlexECS::EntityID> t_processedEntities;
        t_processedEntities.clear();

        for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, Transform>())
        {
            // Check if this entity has already been processed
            if (t_processedEntities.find(entity.Get()) != t_processedEntities.end() || 
                !entity.GetComponent<IsActive>()->is_active)
            {
                continue;  // Skip
            }

            // Traverse up the hierarchy and collect parent entities in a stack
            FlexECS::Entity* t_currentEntity = &entity;

            //Update the parent order
            while (true)
            {
                // Push the entity into the stack to process later
                t_entitystack.push_back(t_currentEntity);

                // Get the parent of the current entity
                Parent* t_parententity = nullptr;
                if ((*t_currentEntity).TryGetComponent<Parent>(t_parententity))
                {
                    t_parententity = (*t_currentEntity).GetComponent<Parent>();
                    // Move up to the parent entity
                    t_currentEntity = &t_parententity->parent;
                }
                else
                {
                    break;
                }
            }
            // At this point, `t_entitystack` contains the chain of entities from the child up to the root

            Matrix4x4 globaltransform = Matrix4x4::Identity;  // Start with identity matrix for the root entity
            for (auto it = t_entitystack.rbegin(); it != t_entitystack.rend(); ++it)
            {
                // Check if the parent has already been processed, don't update matrix
                if (t_processedEntities.find((*it)->Get()) == t_processedEntities.end())
                {
                    //Update current obj transform
                    UpdateTransformationMatrix(**it, globaltransform);
                    // Mark the entity as processed
                    t_processedEntities.insert((*it)->Get());
                }
                // Update the parent's global transform to pass it down to the next child
                globaltransform = (*it)->GetComponent<Transform>()->transform;

                //Log::Debug(FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*(*it)->GetComponent<EntityName>()));

            }
           // Log::Debug(" ");
            t_entitystack.clear();
        }
        //Log::Debug("****************************************************************");
    }

    void RendererSprite2D()
    {
        WindowProps window_props = Application::GetCurrentWindow()->GetProps();
        Renderer2DProps props;
        props.window_size = { static_cast<float>(window_props.width), static_cast<float>(window_props.height) };

        FunctionQueue render_queue;

        if (PostProcessing)
        {
            OpenGLSpriteRenderer::EnablePostProcessing();
            OpenGLSpriteRenderer::ClearFrameBuffer();
        }

        // Render all entities
        for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Position, Scale, Rotation, Transform, Shader, Sprite>())
        {
            if (!entity.GetComponent<IsActive>()->is_active) continue;

            auto& z_index = entity.GetComponent<ZIndex>()->z;
            Matrix4x4 transform = entity.GetComponent<Transform>()->transform;
            auto& shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(entity.GetComponent<Shader>()->shader);
            auto sprite = entity.GetComponent<Sprite>();

            props.shader = shader;
            props.transform = transform;
            props.texture = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(sprite->texture);
            props.color = sprite->color;
            props.color_to_add = sprite->color_to_add;
            props.color_to_multiply = sprite->color_to_multiply;
            props.alignment = static_cast<Renderer2DProps::Alignment>(sprite->alignment);

            //auto testfn = [&props]() { props.transform.Dump(); };
            //testfn();

            render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(props); }, "", z_index });
        }

        // push settings
        bool depth_test = OpenGLRenderer::IsDepthTestEnabled();
        if (depth_test) OpenGLRenderer::DisableDepthTest();

        bool blending = OpenGLRenderer::IsBlendingEnabled();
        if (!blending) OpenGLRenderer::EnableBlending();

        // batch-render

        render_queue.Flush();
        if (PostProcessing)
        {
            OpenGLSpriteRenderer::DisablePostProcessing();
            OpenGLSpriteRenderer::DrawPostProcessingLayer();
        }

        // pop settings

        if (depth_test) OpenGLRenderer::EnableDepthTest();
        if (!blending) OpenGLRenderer::DisableBlending();
    }

}