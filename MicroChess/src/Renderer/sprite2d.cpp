/*!************************************************************************
// WLVERSE [https://wlverse.web.app]
// sprite2d.cpp
//
// This file is responsible for handling 2D sprite rendering within the game. 
// It focuses on batch rendering, enabling the efficient processing of large
// numbers of sprites in a single draw call. The system minimizes OpenGL state
// changes by organizing draw operations into a queue that is flushed once
// all sprites have been prepared.
//
// Key functionalities include:
// - Hierarchical transformation system, where child entities inherit
//   transformations from their parents, enabling complex object groupings
//   and movement.
// - Efficient batch rendering pipeline to reduce draw calls, leveraging
//   modern OpenGL techniques such as VAOs, VBOs, and shader management.
// - Managing the necessary OpenGL resources (shaders, framebuffers, textures)
//   and ensuring optimized memory usage during the rendering process.
// - Ensuring precise alignment of sprites via local transformations and
//   alignment options, with smooth integration into the parent-child matrix
//   relationship.
//
// AUTHORS
// [100%] Soh Wei Jie (weijie.soh@digipen.edu)
//   - Main Author
//   - Developed the hierarchical transformation and batch rendering system
//     to optimize large-scale 2D rendering workflows, including post-processing
//     and shader integration.
//
// Copyright (c) 2024 DigiPen, All rights reserved.
**************************************************************************/

#include "Renderer/sprite2d.h"

#include "Components/rendering.h"

namespace ChronoShift
{
    /*!***************************************************************************
    * \brief
    * Updates the transformation matrix for a given entity based on its local
    * position, scale, and optional rotation. The matrix is combined with the
    * parent's transformation matrix.
    *
    * \param currEntity The entity whose transformation matrix needs to be updated.
    * \param parent_entity_matrix The transformation matrix of the parent entity
    * (defaults to identity matrix).
    *****************************************************************************/
    void UpdateTransformationMatrix(FlexECS::Entity& currEntity, const Matrix4x4 parent_entity_matrix = Matrix4x4::Identity)
    {
        auto& local_transform = currEntity.GetComponent<Transform>()->transform;
        if (!currEntity.GetComponent<Transform>()->is_dirty) return;

        auto& local_position = currEntity.GetComponent<Position>()->position;
        auto& local_scale = currEntity.GetComponent<Scale>()->scale;
        // Get rotation component if it exists
        Rotation* local_rotation = nullptr;
        if (currEntity.TryGetComponent<Rotation>(local_rotation))
            local_rotation = currEntity.GetComponent<Rotation>();

        //Alignment of sprite
        static Vector2 sprite_alignment = Vector2::Zero;
        // Get sprite component if it exists
        Sprite* checkSprite = nullptr;
        Text* checkText = nullptr;
        if (currEntity.TryGetComponent<Sprite>(checkSprite))
        {
            switch (currEntity.GetComponent<Sprite>()->alignment)
            {
            case Renderer2DProps::Alignment::Alignment_TopLeft: sprite_alignment = Vector2(local_scale.x * 0.5f, local_scale.y * 0.5f); break;
            case Renderer2DProps::Alignment::Alignment_TopRight: sprite_alignment = Vector2(-local_scale.x * 0.5f, local_scale.y * 0.5f); break;
            case Renderer2DProps::Alignment::Alignment_BottomLeft: sprite_alignment = Vector2(local_scale.x * 0.5f, -local_scale.y * 0.5f); break;
            case Renderer2DProps::Alignment::Alignment_BottomRight: sprite_alignment = Vector2(-local_scale.x * 0.5f, -local_scale.y * 0.5f); break;
            default: case Renderer2DProps::Alignment::Alignment_Center: sprite_alignment = Vector2::Zero; break;
            }
        }
        //else if (currEntity.TryGetComponent<Text>(checkText))
        //{
        //    //TODO DOUBLE CHECK THIS PART (SEEMS VERY INEFFICIENT)
        //    //CHANGE TO BITWISE FOR BETTER ADAPTABILITY
        //    switch (currEntity.GetComponent<Text>()->alignment)
        //    {
        //    case Renderer2DProps::Alignment::Alignment_TopLeft: sprite_alignment = Vector2(local_scale.x * 0.5f, local_scale.y * 0.5f); break;
        //    case Renderer2DProps::Alignment::Alignment_TopRight: sprite_alignment = Vector2(-local_scale.x * 0.5f, local_scale.y * 0.5f); break;
        //    case Renderer2DProps::Alignment::Alignment_BottomLeft: sprite_alignment = Vector2(local_scale.x * 0.5f, -local_scale.y * 0.5f); break;
        //    case Renderer2DProps::Alignment::Alignment_BottomRight: sprite_alignment = Vector2(-local_scale.x * 0.5f, -local_scale.y * 0.5f); break;
        //    default: case Renderer2DProps::Alignment::Alignment_Center: sprite_alignment = Vector2::Zero; break;
        //    }
        //}

        // calculate the transform
        Matrix4x4 translation_matrix = Matrix4x4::Translate(Matrix4x4::Identity, Vector3(-(local_position.x + sprite_alignment.x), local_position.y + sprite_alignment.y, 0.0f));
        Matrix4x4 rotation_matrix = Quaternion::FromEulerAnglesDeg(local_rotation != nullptr ? local_rotation->rotation : Vector3::Zero).ToRotationMatrix();
        Matrix4x4 scale_matrix = Matrix4x4::Scale(Matrix4x4::Identity, local_scale);

        // Apply parent entity's matrix
        local_transform = parent_entity_matrix * (translation_matrix * rotation_matrix * scale_matrix);
    }

    /*!***************************************************************************
    * \brief
    * Updates the transformation matrix of 2D sprites within the scene. It ensures
    * proper alignment and processing of entities in the scene, particularly
    * their position and orientation in the hierarchy.
    *****************************************************************************/
    void UpdateSprite2DMatrix()
    {
        //DEBUG CHECKS IF IMAGE IS FROZEN OR NOT SHOWING
        // 1. DID YOU SET IS_DIRTY = true;
        // 2. DO YOU ADD A TRANSFORM COMPONENT TO YOUR OBJECT
        // 3. IS YOUR OBJECTS BEING CALLED IN THE LOOP TO BE PROCESSED? //Uncomment logs to check or ask wei jie

        static std::vector<FlexECS::Entity*> t_entitystack;
        t_entitystack.clear();  // Clear stack at the beginning of each update
        // Unordered set to track processed entities (avoid redundant updates)
        static std::unordered_set<FlexECS::EntityID> t_processedEntities;
        t_processedEntities.clear();

        for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, Transform>())
        {
            // Check if this entity has already been processed
            if (t_processedEntities.find(entity.Get()) != t_processedEntities.end() || !entity.GetComponent<IsActive>()->is_active) continue; //Skip

            // Traverse up the hierarchy and collect parent entities in a stack
            FlexECS::Entity* t_currentEntity = &entity;
            // Track whether any entity in the stack is dirty
            bool entity_isdirty = false;
            //Update the parent order
            while (true)
            {
                // Push the entity into the stack to process later
                t_entitystack.push_back(t_currentEntity);
                entity_isdirty = t_currentEntity->GetComponent<Transform>()->is_dirty ? true : entity_isdirty;

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
                    // If any entity in the hierarchy is dirty, mark all as dirty
                    (*it)->GetComponent<Transform>()->is_dirty = entity_isdirty ? true : (*it)->GetComponent<Transform>()->is_dirty;
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

        //Ensure all entities is no longer dirty
        for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, Transform>())
        {
            if (!entity.GetComponent<IsActive>()->is_active) continue;

            entity.GetComponent<Transform>()->is_dirty = false;
        }
    }

    /*!***************************************************************************
    * \brief
    * Renders all the 2D sprites in the scene. This function handles the necessary
    * steps for rendering, including setting up shader properties, handling
    * post-processing, and batch rendering for efficiency.
    *****************************************************************************/
    void RendererSprite2D()
    {
        //TODO: Run through Update Queue instead of checking for bool isDirty
        //Update Transformation Matrix of All Entities
        UpdateSprite2DMatrix();

        WindowProps window_props = Application::GetCurrentWindow()->GetProps();
        Renderer2DProps props;
        props.window_size = { static_cast<float>(window_props.width), static_cast<float>(window_props.height) };

        ////////////////////////////////////////////////////////////////////////////////
        // Potential Issues
        ////////////////////////////////////////////////////////////////////////////////
        // 1. the order of post-processed objects is rendered first, then non-post-processed (For the sake of text box)
        //Test Debug:
        //auto start = std::chrono::high_resolution_clock::now();
        //auto end = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        //std::cout << "Time taken: " << duration << " ms" << std::endl;
        
        FunctionQueue finalized_render_queue;
        // Render all entities via normal draw call(no batching)
        #if 0
        {
            FunctionQueue pp_render_queue, non_pp_render_queue;

            //same thing as update transformations, flush update queue (lags if more than 2500 objects)
            for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Transform, Shader, Sprite>())
            {
                auto entity_name_component = entity.GetComponent<EntityName>();

                if (!entity.GetComponent<IsActive>()->is_active) continue;
                auto& z_index = entity.GetComponent<ZIndex>()->z;
                Matrix4x4 transform = entity.GetComponent<Transform>()->transform;
                auto& shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(entity.GetComponent<Shader>()->shader);
                auto sprite = entity.GetComponent<Sprite>();

                props.shader = shader;
                props.transform = transform;
                props.texture = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(sprite->texture);
                //props.color = sprite->color;
                props.color_to_add = sprite->color_to_add;
                props.color_to_multiply = sprite->color_to_multiply;
                props.alignment = static_cast<Renderer2DProps::Alignment>(sprite->alignment);
                props.vbo_id = sprite->vbo_id;

                //FIRST TWO IFS SHOULD NOT BE HERE
                if ("finalRender" == FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity_name_component))
                    finalized_render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(OpenGLSpriteRenderer::GetCreatedTexture(OpenGLSpriteRenderer::CID_editor),props); }, "", z_index });
                else if ("editorRender" == FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity_name_component))
                    finalized_render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(OpenGLSpriteRenderer::GetCreatedTexture(OpenGLSpriteRenderer::CID_finalRender),props); }, "", z_index });
                else if (sprite->post_processed)
                    pp_render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(props); }, "", z_index });
                else
                    non_pp_render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(props); }, "", z_index });

            }

            //Push Settings
            bool depth_test = OpenGLRenderer::IsDepthTestEnabled();
            if (depth_test) OpenGLRenderer::DisableDepthTest();

            bool blending = OpenGLRenderer::IsBlendingEnabled();
            if (!blending) OpenGLRenderer::EnableBlending();



            //Batch Rendering objs in scene
            {
                // Set up Editor Frame Buffer for batch-rendering in the editor
                OpenGLSpriteRenderer::SetEditorFrameBuffer();
                OpenGLSpriteRenderer::ClearFrameBuffer();

                // Render post-processing objects as the background layer
                pp_render_queue.Flush();
                OpenGLSpriteRenderer::DrawPostProcessingLayer();

                // Render non-post-processing objects on top
                non_pp_render_queue.Flush();
            }

            // Switch to default frame buffer for final output rendering
            OpenGLSpriteRenderer::SetDefaultFrameBuffer();
            finalized_render_queue.Flush();  // Final rendering (UI, etc.)

            //auto end = std::chrono::high_resolution_clock::now();
            //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            //std::cout << "Time taken: " << duration << " ms" << std::endl;

            // pop settings
            if (depth_test) OpenGLRenderer::EnableDepthTest();
            if (!blending) OpenGLRenderer::DisableBlending();
        }
        #endif

        // Integrating batching to pipeline
        #if 1
        {
            std::unordered_map<std::string, Sprite_Batch_Inst> batchMap;

            //same thing as update transformations, flush update queue (lags if more than 2500 objects)
            for (auto& entity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Transform, Shader, Sprite>())
            {
                auto entity_name_component = entity.GetComponent<EntityName>();

                if (!entity.GetComponent<IsActive>()->is_active) continue;

                //auto& z_index = entity.GetComponent<ZIndex>()->z;
                Matrix4x4 transform = entity.GetComponent<Transform>()->transform;
                auto& shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(entity.GetComponent<Shader>()->shader);
                auto sprite = entity.GetComponent<Sprite>();

                //Need for editor tex (Remove pls)
                props.shader = shader;
                props.transform = transform;
                //props.texture 
                props.color_to_add = sprite->color_to_add;
                props.color_to_multiply = sprite->color_to_multiply;
                props.alignment = static_cast<Renderer2DProps::Alignment>(sprite->alignment);
                props.vbo_id = sprite->vbo_id;

                //FIRST TWO IFS SHOULD NOT BE HERE
                if ("finalRender" == FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity_name_component))
                {
                    finalized_render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(OpenGLSpriteRenderer::GetCreatedTexture(OpenGLSpriteRenderer::CID_editor),props); }, "", 1 });
                    continue;
                }
                else if ("editorRender" == FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(*entity_name_component))
                {
                    finalized_render_queue.Insert({ [props]() { OpenGLSpriteRenderer::DrawTexture2D(OpenGLSpriteRenderer::GetCreatedTexture(OpenGLSpriteRenderer::CID_finalRender),props); }, "", 0 });
                    continue;
                }

                // Use texture as the unique key for batching (or combine multiple properties as needed)
                std::string batchKey = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(sprite->texture);
                // If a batch for this sprite type doesn't exist, create it
                if (batchMap.find(batchKey) == batchMap.end())
                {
                    batchMap[batchKey] = Sprite_Batch_Inst();
                    batchMap[batchKey].m_vboid = sprite->vbo_id;
                }
                // Push transformation and color data to the Sprite_Batch_Inst
                batchMap[batchKey].m_transformationData.push_back(transform);
                batchMap[batchKey].m_colorAddData.push_back(props.color_to_add);
                batchMap[batchKey].m_colorMultiplyData.push_back(props.color_to_multiply);


            }

            //Push Settings
            bool depth_test = OpenGLRenderer::IsDepthTestEnabled();
            if (depth_test) OpenGLRenderer::DisableDepthTest();

            bool blending = OpenGLRenderer::IsBlendingEnabled();
            if (!blending) OpenGLRenderer::EnableBlending();

            //Batch Rendering objs in scene
            {
                // Set up Editor Frame Buffer for batch-rendering in the editor
                OpenGLSpriteRenderer::SetEditorFrameBuffer();
                OpenGLSpriteRenderer::ClearFrameBuffer();

                for (auto& [key, batchData] : batchMap)
                {
                    props.texture = key;
                    props.vbo_id = batchData.m_vboid;

                    // Draw batch for this unique sprite
                    OpenGLSpriteRenderer::DrawBatchTexture2D(props, batchData);
                }
                OpenGLSpriteRenderer::DrawPostProcessingLayer();
            }

            // Switch to default frame buffer for final output rendering
            OpenGLSpriteRenderer::SetDefaultFrameBuffer();
            finalized_render_queue.Flush();  // Final rendering (UI, etc.)

            // pop settings
            if (depth_test) OpenGLRenderer::EnableDepthTest();
            if (!blending) OpenGLRenderer::DisableBlending();
        }
        #endif

        #if 1
        {

            FunctionQueue text_render_queue;
            Renderer2DText sample;
            for (auto& txtentity : FlexECS::Scene::GetActiveScene()->View<IsActive, ZIndex, Transform, Shader, Text>())
            {
                /////////////////////////////////////
                //How to retrieve
                /////////////////////////////////////
               //auto i = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(txtentity.GetComponent<Text>()->fonttype);
               //auto& asset_font = FLX_ASSET_GET(Asset::Font, FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(txtentity.GetComponent<Text>()->fonttype));

                if (!txtentity.GetComponent<IsActive>()->is_active) continue;
                Matrix4x4 transform = txtentity.GetComponent<Transform>()->transform;
                auto shader = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(txtentity.GetComponent<Shader>()->shader);
                auto font = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(txtentity.GetComponent<Text>()->fonttype);
                
                sample.m_words = FlexECS::Scene::GetActiveScene()->Internal_StringStorage_Get(txtentity.GetComponent<Text>()->text);
                sample.m_shader = shader;
                sample.m_fonttype = font;
                sample.m_transform = transform;
                sample.m_alignment = {
                    static_cast<Renderer2DText::AlignmentX>(txtentity.GetComponent<Text>()->alignment.first),
                    static_cast<Renderer2DText::AlignmentY>(txtentity.GetComponent<Text>()->alignment.second)
                };
                sample.m_color = txtentity.GetComponent<Text>()->color;
                
                text_render_queue.Insert({ [sample]() { OpenGLTextRenderer::DrawText2D(sample); }, "", 0 });
            }

            //Push Settings
            bool depth_test = OpenGLRenderer::IsDepthTestEnabled();
            if (depth_test) OpenGLRenderer::DisableDepthTest();

            bool blending = OpenGLRenderer::IsBlendingEnabled();
            if (!blending) OpenGLRenderer::EnableBlending();

            OpenGLSpriteRenderer::SetDefaultFrameBuffer();
            text_render_queue.Flush();  // Final rendering (UI, etc.)

            // pop settings
            if (depth_test) OpenGLRenderer::EnableDepthTest();
            if (!blending) OpenGLRenderer::DisableBlending();
            #endif
        }
    }
}