#include "cMesh.h"
#include <extern/glm/glm.hpp>
#include <extern/glm/mat4x4.hpp>
#include <extern/glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <extern/glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "Graphics/GLCommon.h"       // Gives us glad.h
#include "Graphics/cVAOManager.h"
#include "globals.h"


void GLFW_window_size_callback(GLFWwindow* window, int width, int height)
{
    // TODO: GLFW_window_size_callback()

    return;
}

// HACK: We shouldn't do the getUniformLocation every frame.
//  These don't change, so we should store them outside    
void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgram)
{

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[0] >= 0.0f)
    {
        // uniform sampler2D texture_00;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Fauci.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[0]);

        GLuint textureUnit = 0;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_00_LocID = g_GetUniformLocation(shaderProgram, "texture_00");
        glUniform1i(texture_00_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[1] >= 0.0f)
    {
        // uniform sampler2D texture_01;
        //GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Pebbleswithquarzite.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[1]);

        GLuint textureUnit = 1;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_01_LocID = g_GetUniformLocation(shaderProgram, "texture_01");
        glUniform1i(texture_01_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[2] >= 0.0f)
    {
        // uniform sampler2D texture_01;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Lisse_mobile_shipyard-mal1.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[2]);

        GLuint textureUnit = 2;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_02_LocID = g_GetUniformLocation(shaderProgram, "texture_02");
        glUniform1i(texture_02_LocID, textureUnit);
    }
    //*****************************************************************************************    

    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[3] >= 0.0f) {
        // uniform sampler2D texture_01;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Broc_tree_house.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[3]);

        GLuint textureUnit = 3;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_03_LocID = g_GetUniformLocation(shaderProgram, "texture_03");
        glUniform1i(texture_03_LocID, textureUnit);
    }
    //*****************************************************************************************    
    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[4] >= 0.0f) {
        // uniform sampler2D maskTexture_00;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Broc_tree_house.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[4]);

        GLuint textureUnit = 10;			// Texture unit go from 0 to 79. 10-20 are being used for masks.
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint maskTexture_00_LocID = g_GetUniformLocation(shaderProgram, "maskTexture_00");
        glUniform1i(maskTexture_00_LocID, textureUnit);

        GLuint TextureNumber2 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[6]);

        GLuint textureUnit2 = 20;			// Texture unit go from 0 to 79. 20-30 are being used for textures to be used with masks.
        glActiveTexture(textureUnit2 + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber2);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint maskTextureTop_00_LocID = g_GetUniformLocation(shaderProgram, "maskTextureTop_00");
        glUniform1i(maskTextureTop_00_LocID, textureUnit2);
    }
    //*****************************************************************************************    
    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[5] >= 0.0f) {
        // uniform sampler2D maskTexture_01;
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[5]);

        GLuint textureUnit = 11;			// Texture unit go from 0 to 79. 10-19 are being used for masks.
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint maskTexture_01_LocID = g_GetUniformLocation(shaderProgram, "maskTexture_01");
        glUniform1i(maskTexture_01_LocID, textureUnit);

        GLuint TextureNumber2 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[7]);

        GLuint textureUnit2 = 21;			// Texture unit go from 0 to 79. 20-39 are being used for textures to be used with masks.
        glActiveTexture(textureUnit2 + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber2);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint maskTextureTop_01_LocID = g_GetUniformLocation(shaderProgram, "maskTextureTop_01");
        glUniform1i(maskTextureTop_01_LocID, textureUnit2);
    }
    //*****************************************************************************************      

    // Set all the texture ratios in the shader
    GLint textureRatios0to3_LocID = g_GetUniformLocation(shaderProgram, "texture2D_Ratios0to3");
    // Set them
    glUniform4f(textureRatios0to3_LocID,
        pCurrentMesh->textureRatios[0],
        pCurrentMesh->textureRatios[1],
        pCurrentMesh->textureRatios[2],
        pCurrentMesh->textureRatios[3]);

    // Set all the texture ratios in the shader
    GLint mask_Ratios0to1_LocID = g_GetUniformLocation(shaderProgram, "mask_Ratios0to1");
    // Set them. Only use two for now, but utilizing vec4 in case we wish to expand.
    glUniform4f(mask_Ratios0to1_LocID,
        pCurrentMesh->textureRatios[4],
        pCurrentMesh->textureRatios[5],
        0,
        0);

    {
        GLint bDiscardTransparencyWindowsOn_LodID = g_GetUniformLocation(shaderProgram, "bDiscardTransparencyWindowsOn");
        // Turn discard transparency off
        glUniform1f(bDiscardTransparencyWindowsOn_LodID, (GLfloat)GL_FALSE);

        if (pCurrentMesh->bUseDiscardTransparency)
        {
            //                GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName("Lisse_mobile_shipyard-mal1.bmp");
            GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[8]);
            // Picking texture unit 30 since it's not in use.
            GLuint discardTextureUnit = 30;			// Texture unit go from 0 to 79
            glActiveTexture(discardTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
            glBindTexture(GL_TEXTURE_2D, discardTextureNumber);
            GLint discardTexture_LocID = g_GetUniformLocation(shaderProgram, "discardTexture");
            glUniform1i(discardTexture_LocID, discardTextureUnit);

            GLint discardColour_LocID = g_GetUniformLocation(shaderProgram, "discardColour");
            glUniform3f(discardColour_LocID, pCurrentMesh->discardColour.r, pCurrentMesh->discardColour.g, pCurrentMesh->discardColour.b);

            // Turn discard function on
            glUniform1f(bDiscardTransparencyWindowsOn_LodID, (GLfloat)GL_TRUE);
        }
    }

    {
        GLint eyeLocation_LocID = g_GetUniformLocation(shaderProgram, "eyeLocation");
        glUniform4f(eyeLocation_LocID, ::g_pActiveCamera->eye.x, ::g_pActiveCamera->eye.y, ::g_pActiveCamera->eye.z, 1.f);

        GLint bDoesReflect_LodID = g_GetUniformLocation(shaderProgram, "bDoesReflect");
        // Turn discard transparency off
        glUniform1f(bDoesReflect_LodID, (GLfloat)GL_FALSE);

        if (pCurrentMesh->bDoesReflect)
        {
            // Turn reflection on
            glUniform1f(bDoesReflect_LodID, (GLfloat)GL_TRUE);
        }

        GLint bDoesReract_LodID = g_GetUniformLocation(shaderProgram, "bDoesRefract");
        // Turn discard transparency off
        glUniform1f(bDoesReract_LodID, (GLfloat)GL_FALSE);
        if (pCurrentMesh->bDoesRefract)
        {
            GLint refraction_LodID = g_GetUniformLocation(shaderProgram, "fRefractionIndex");
            glUniform1f(refraction_LodID, pCurrentMesh->refractionIndex);
            // Turn refraction on
            glUniform1f(bDoesReract_LodID, (GLfloat)GL_TRUE);
        }
    }

    GLint bUseSpecularMap_Location = g_GetUniformLocation(shaderProgram, "bUseSpecularMap");

    if (pCurrentMesh->bUseSpecularMap)
    {
        glUniform1f(bUseSpecularMap_Location, (float)GL_TRUE);
        GLuint specMapTextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->specularMapTexture);
        // Picking texture unit 31 since it's not in use.
        GLuint specMapTextureUnit = 71;			// Texture unit go from 0 to 79
        glActiveTexture(specMapTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, specMapTextureNumber);
        GLint specMapTexture_LocID = g_GetUniformLocation(shaderProgram, "specularMapTexture");
        glUniform1i(specMapTexture_LocID, specMapTextureUnit);
    }
    else{
        glUniform1f(bUseSpecularMap_Location, (float)GL_FALSE);
    }

    GLuint staticTextureNumber = ::g_pTextureManager->getTextureIDFromName(::g_pEffectsManager->GetStaticTexture(pCurrentMesh->getUniqueID()));
    // Picking texture unit 72 since it's not in use.
    GLuint staticTextureUnit = 72;			// Texture unit go from 0 to 79
    glActiveTexture(staticTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, staticTextureNumber);
    GLint staticTexture_LocID = g_GetUniformLocation(shaderProgram, "staticTexture");
    glUniform1i(staticTexture_LocID, staticTextureUnit);

    GLint flickerAmount_LodID = g_GetUniformLocation(shaderProgram, "flickerAmount");
    // Set the flicker amount
    glUniform1f(flickerAmount_LodID, ::g_pEffectsManager->GetFlickerAmount(pCurrentMesh->getUniqueID()));


    {
        //GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[3]);
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("WinterRiver");

        // Be careful that you don't mix up the 2D and the cube assignments for the texture units
        //
        // Here, I'll say that the cube maps start at texture unit 50
        //
        GLuint textureUnit = 50;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

        // ***NOTE*** Binding to a CUBE MAP not a 2D Texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint cubeMap_00_LocID = g_GetUniformLocation(shaderProgram, "cubeMap_00");
        glUniform1i(cubeMap_00_LocID, textureUnit);

    } 

    {   // Set up the blend values for the skyboxes
        GLint cubeMap_Ratios0to3_LocID = g_GetUniformLocation(shaderProgram, "cubeMap_Ratios0to3");

        float howMuch_cubeMap_00 = 1.0f;
        float howMuch_cubeMap_01 = 0.0f;
        float howMuch_cubeMap_02 = 0.0f;
        float howMuch_cubeMap_03 = 0.0f;

        // Blend the maps (we are currently only using the first one).
        glUniform4f(
            cubeMap_Ratios0to3_LocID,
            howMuch_cubeMap_00,       
            howMuch_cubeMap_01,       
            howMuch_cubeMap_02,
            howMuch_cubeMap_03);
    } 

    return;
}


void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel,
                GLint matModel_Location,
                GLint matModelInverseTranspose_Location,
                GLuint program,
                cVAOManager* pVAOManager)
{
    // Set up textures for this object
    SetUpTextures(pCurrentMesh, program);

    // Alpha transparency
    glEnable(GL_BLEND);
    // Basic "alpha transparency"
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get the uniform (should be outside of the draw call)
    GLint wholeObjectAlphaTransparency_LocID = g_GetUniformLocation(program, "wholeObjectAlphaTransparency");
    // Set this value here
    glUniform1f(wholeObjectAlphaTransparency_LocID, pCurrentMesh->alphaTransparency);

    // *****************************************************
    // Translate or "move" the object somewhere
    glm::mat4 matTranslate = glm::translate( glm::mat4(1.0f),
                                                pCurrentMesh->positionXYZ );
    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate( glm::mat4(1.0f),
                                        pCurrentMesh->orientationXYZ.z,//(float)glfwGetTime(),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = matModel * rotateZ;
    // *****************************************************

    // *****************************************************
    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
                                    pCurrentMesh->orientationXYZ.y,
                                    glm::vec3(0.0f, 1.0f, 0.0f));
    //matModel = matModel * rotateY;
    // *****************************************************
            
    // *****************************************************
    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
                                    pCurrentMesh->orientationXYZ.x,
                                    glm::vec3(1.0f, 0.0f, 0.0f));
    //matModel = matModel * rotateX;
    // *****************************************************

            
    // *****************************************************
    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                    glm::vec3(pCurrentMesh->scale * pCurrentMesh->nonUniformScale.x,  // Scale in X
                                                pCurrentMesh->scale * pCurrentMesh->nonUniformScale.y,  // Scale in Y
                                                pCurrentMesh->scale * pCurrentMesh->nonUniformScale.z));// Scale in Z
    //matModel = matModel * matScale;
    // *****************************************************

    // *****************************************************
    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;     // <-- mathematically, this is 1st

    // Now the matModel ("Model" or "World") matrix
    //  represents ALL the transformations we want, in ONE matrix.

    // Moved view and projection ("v" and "p") to outside draw scene loop.
    // (because they are the same for all objects)

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));


    // Inverse transpose of the model matrix
    // (Used to calculate the normal location in vertex space, using only rotation)
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));


    // Copy the whole object colour information to the sahder               

    // This is used for wireframe or whole object colour. 
    // If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
    GLint bUseDebugColour_Location = g_GetUniformLocation(program, "bUseDebugColour");
    GLint objectDebugColour_Location = g_GetUniformLocation(program, "objectDebugColour");

    // If true, then the lighting contribution is NOT used. 
    // This is useful for wireframe object
    GLint bDontLightObject_Location = g_GetUniformLocation(program, "bDontLightObject");

    // The "whole object" colour (diffuse and specular)
    GLint wholeObjectDiffuseColour_Location = g_GetUniformLocation(program, "wholeObjectDiffuseColour");
    GLint bUseWholeObjectDiffuseColour_Location = g_GetUniformLocation(program, "bUseWholeObjectDiffuseColour");
    GLint wholeObjectSpecularColour_Location = g_GetUniformLocation(program, "wholeObjectSpecularColour");

    if (pCurrentMesh->bUseWholeObjectDiffuseColour)
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_TRUE);
        glUniform4f(wholeObjectDiffuseColour_Location, 
                    pCurrentMesh->wholeObjectDiffuseRGBA.r,
                    pCurrentMesh->wholeObjectDiffuseRGBA.g,
                    pCurrentMesh->wholeObjectDiffuseRGBA.b,
                    pCurrentMesh->wholeObjectDiffuseRGBA.a);
    }
    else
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);
    }

    glUniform4f(wholeObjectSpecularColour_Location, 
                pCurrentMesh->wholeObjectSpecularRGB.r,
                pCurrentMesh->wholeObjectSpecularRGB.g,
                pCurrentMesh->wholeObjectSpecularRGB.b,
                pCurrentMesh->wholeObjectShininess_SpecPower);


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (pCurrentMesh->bUseObjectDebugColour)
    {
        // Override the colour...
        glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
        glUniform4f(objectDebugColour_Location,
                    pCurrentMesh->objectDebugColourRGBA.r, 
                    pCurrentMesh->objectDebugColourRGBA.g,
                    pCurrentMesh->objectDebugColourRGBA.b,
                    pCurrentMesh->objectDebugColourRGBA.a);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
    }


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (pCurrentMesh->bDontLight)
    {
        // Override the colour...
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }


    // Wireframe
    if (pCurrentMesh->bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Draw everything with only lines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    sModelDrawInfo modelInfo;
//        if (gVAOManager.FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
//        if (gVAOManager.FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

    if ( pVAOManager->FindDrawInfoByModelName( pCurrentMesh->meshName, modelInfo) )
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
                        modelInfo.numberOfIndices, 
                        GL_UNSIGNED_INT, 
                        (void*)0);

        glBindVertexArray(0);
    }


    return;
}
