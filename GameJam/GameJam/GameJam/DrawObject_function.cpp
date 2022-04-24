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

        GLint texture_03_LocID = g_GetUniformLocation(shaderProgram, "texture_03");
        glUniform1i(texture_03_LocID, textureUnit);
    }
    //*****************************************************************************************    
    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[4] >= 0.0f) {
        // uniform sampler2D maskTexture_00;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Broc_tree_house.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[4]);

        GLuint textureUnit = 20;			// Texture unit go from 0 to 79. 20-30 are being used for masks.
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint maskTexture_00_LocID = g_GetUniformLocation(shaderProgram, "maskTexture_00");
        glUniform1i(maskTexture_00_LocID, textureUnit);

        GLuint TextureNumber2 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[6]);

        GLuint textureUnit2 = 21;			// Texture unit go from 0 to 79. 20-30 are being used for textures to be used with masks.
        glActiveTexture(textureUnit2 + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber2);

        GLint maskTextureTop_00_LocID = g_GetUniformLocation(shaderProgram, "maskTextureTop_00");
        glUniform1i(maskTextureTop_00_LocID, textureUnit2);
    }
    //*****************************************************************************************    
    //*****************************************************************************************
    if (pCurrentMesh->textureRatios[5] >= 0.0f) {
        // uniform sampler2D maskTexture_01;
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[5]);

        GLuint textureUnit = 22;			// Texture unit go from 0 to 79. 10-19 are being used for masks.
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint maskTexture_01_LocID = g_GetUniformLocation(shaderProgram, "maskTexture_01");
        glUniform1i(maskTexture_01_LocID, textureUnit);

        GLuint TextureNumber2 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[6]);

        GLuint textureUnit2 = 23;			// Texture unit go from 0 to 79. 20-30 are being used for textures to be used with masks.
        glActiveTexture(textureUnit2 + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber2);

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
            GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[8]);
            // Picking texture unit 30 since it's not in use.
            GLuint discardTextureUnit = 30;			// Texture unit go from 0 to 79
            glActiveTexture(discardTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
            glBindTexture(GL_TEXTURE_2D, discardTextureNumber);
            GLint discardTexture_LocID = g_GetUniformLocation(shaderProgram, "discardTexture");
            glUniform1i(discardTexture_LocID, discardTextureUnit);

            // Turn discard function on
            glUniform1f(bDiscardTransparencyWindowsOn_LodID, (GLfloat)GL_TRUE);

            GLint discardColour_LocID = g_GetUniformLocation(shaderProgram, "discardColour");
            glUniform4f(discardColour_LocID,
                pCurrentMesh->discardColour.r,
                pCurrentMesh->discardColour.g,
                pCurrentMesh->discardColour.b,
                1.f);
        }
    }

    GLint bUseSpecularMap_Location = glGetUniformLocation(shaderProgram, "bUseSpecularMap");

    if (pCurrentMesh->bUseSpecularMap)
    {
        glUniform1f(bUseSpecularMap_Location, (float)GL_TRUE);
        GLuint specMapTextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->specularMapTexture);
        // Picking texture unit 31 since it's not in use.
        GLuint specMapTextureUnit = 71;			// Texture unit go from 0 to 79
        glActiveTexture(specMapTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, specMapTextureNumber);
        GLint specMapTexture_LocID = glGetUniformLocation(shaderProgram, "specularMapTexture");
        glUniform1i(specMapTexture_LocID, specMapTextureUnit);
    }
    else {
        glUniform1f(bUseSpecularMap_Location, (float)GL_FALSE);
    }

    {
        //Crosshairs
        GLint bShowCrossHair_LodID = g_GetUniformLocation(shaderProgram, "bShowCrosshair");
        if (::g_pGameEngine->g_pGameplayManager->GetAiming()) {
            glUniform1f(bShowCrossHair_LodID, (GLfloat)GL_TRUE);
        }
        else {
            glUniform1f(bShowCrossHair_LodID, (GLfloat)GL_FALSE);
        }
        GLuint crosshairTextureNumber = ::g_pTextureManager->getTextureIDFromName("crosshair.bmp"); 
        GLuint crosshairTextureUnit = 31;			// Texture unit go from 0 to 79
        glActiveTexture(crosshairTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, crosshairTextureNumber);
        GLint crosshairTexture_LocID = g_GetUniformLocation(shaderProgram, "crosshair");
        glUniform1i(crosshairTexture_LocID, crosshairTextureUnit);
    }
    {
        GLuint waterTextureNumber = ::g_pTextureManager->getTextureIDFromName("UnderwaterNormals.bmp");
        GLuint waterTextureUnit = 32;			// Texture unit go from 0 to 79
        glActiveTexture(waterTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, waterTextureNumber);
        GLint crosshairTexture_LocID = g_GetUniformLocation(shaderProgram, "waterTexture");
        glUniform1i(crosshairTexture_LocID, waterTextureUnit);
    }

    {
        //GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[3]);
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Skybox");

        // Be careful that you don't mix up the 2D and the cube assignments for the texture units
        //
        // Here, I'll say that the cube maps start at texture unit 40
        //
        GLuint textureUnit = 40;			// Texture unit go from 0 to 79
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

std::string GetAppropriateModel(cMesh* pCurrentMesh) {
    float distance = glm::distance(pCurrentMesh->positionXYZ, ::g_pFlyCamera->getEye());
    if (distance > 400.f) {
        return pCurrentMesh->lowDetailMeshName;
    }
    else if (distance > 200.f) {
        return pCurrentMesh->midDetailMeshName;
    }
    else if (distance > 100.f) {
        return pCurrentMesh->highDetailMeshName;
    }
    return pCurrentMesh->meshName;
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
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        pCurrentMesh->positionXYZ);
    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
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
        pCurrentMesh->scale);// Scale
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
        glPolygonMode(GL_FRONT, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT, GL_FILL);
    }

    GLint useBones_LocID = glGetUniformLocation(program, "bUseBones");
    if (pCurrentMesh->bUseBones) {

        glUniform1f(useBones_LocID, (float)GL_TRUE);
        // Send Bone values
        std::vector<GLuint> BoneMatrixLocations;
        std::vector<GLuint> BoneRotationMatrixLocations;
        std::string location = "";
        std::string rotationLocation = "";
        for (int i = 0; i < 40; i++)
        {
            location = "BoneMatrices[" + std::to_string(i) + "]";
            BoneMatrixLocations.push_back(glGetUniformLocation(program, location.c_str()));
            rotationLocation = "BoneRotationMatrices[" + std::to_string(i) + "]";
            BoneRotationMatrixLocations.push_back(glGetUniformLocation(program, rotationLocation.c_str()));
            glUniformMatrix4fv(BoneMatrixLocations[i], 1, GL_FALSE, glm::value_ptr(pCurrentMesh->meshData->mBoneData[i].FinalTransformation));

            glm::mat4 rotation = glm::mat4(glm::quat(pCurrentMesh->meshData->mBoneData[i].FinalTransformation));
            glUniformMatrix4fv(BoneRotationMatrixLocations[i], 1, GL_FALSE, glm::value_ptr(rotation));
        }
    }
    else {
        glUniform1f(useBones_LocID, (float)GL_FALSE);
    }

    GLint eye_Location = g_GetUniformLocation(program, "eyeLocation");
    glUniform4f(eye_Location,
        ::g_pFlyCamera->getEye().x,
        ::g_pFlyCamera->getEye().y,
        ::g_pFlyCamera->getEye().z,
        1.f);

    GLint delta_Location = g_GetUniformLocation(program, "deltaTime");
    glUniform1f(delta_Location, ::g_deltaTime);

    GLint runTime_Location = g_GetUniformLocation(program, "totalTime");
    glUniform1f(runTime_Location, ::g_runTime);

    sModelDrawInfo modelInfo;

    if (pVAOManager->FindDrawInfoByModelName(GetAppropriateModel(pCurrentMesh), modelInfo))
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            modelInfo.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0);
    }

    // Now draw all the "child" objects.
    for (std::vector< cMesh* >::iterator itChildMesh = pCurrentMesh->vec_pChildMeshes.begin();
        itChildMesh != pCurrentMesh->vec_pChildMeshes.end(); itChildMesh++)
    {

        cMesh* pChildMesh = *itChildMesh;

        // Call DrawObject again, but I pass the PARENT's final model matrix value
        // Which will make the child's "origin" at the final location of the parent. 
        DrawObject(pChildMesh,
            matModel,           // The parent's final mat model, not an identity mat4
            matModel_Location,
            matModelInverseTranspose_Location,
            program,
            pVAOManager);

    }//for (cMesh*::iterator itChildMesh...

    return;
}
