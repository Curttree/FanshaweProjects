#include "cLightManager.h"

#include "GLCommon.h"

#include <sstream>
#include <string>

cLightManager::cLightManager()
{
	// Set all the values of the lights, etc.
}

/*static*/ 
cLightHelper cLightManager::lightHelper;

sLight::sLight()
{
	// Clear all the values to the unknown ones
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	// White light
	this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// White specular highlight
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	this->atten = glm::vec4(0.0f, 0.1f, 0.01f, 100000.0f);

	this->direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

	// x = lightType, y = inner angle, z = outer angle, w = TBD
	// 0 = pointlight
	// 1 = spot light
	// 2 = directional light
	this->param1.x = 0;	// Spot light
	this->param1.y = 0.0f;
	this->param1.z = 0.0f;
	this->param1.w = 1.0f;	// not used, so set to 1.0f

	// x = 0 for off, 1 for on
	this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);	

	// Here's the uniform locations of the light values in the shader
	// Settting these to -1 which is the "not found" or unknown uniform location
	this->position_uniform_location = -1;
	this->diffuse_uniform_location = -1;
	this->specular_uniform_location = -1;
	this->atten_uniform_location = -1;
	this->direction_uniform_location = -1;
	this->param1_uniform_location = -1;
	this->param2_uniform_location = -1;
}



void cLightManager::TurnOnLight(unsigned int lightNumber)
{
	if (lightNumber < cLightManager::NUMBER_OF_LIGHTS)
	{
		this->theLights[lightNumber].param2.x = 1.0f;
	}
	return;
}

void cLightManager::TurnOffLight(unsigned int lightNumber)
{
	if (lightNumber < cLightManager::NUMBER_OF_LIGHTS)
	{
		this->theLights[lightNumber].param2.x = 0.0f;
	}
	return;
}

void cLightManager::SetLightColour(unsigned int lightNumber, float red, float green, float blue) {
	theLights[lightNumber].diffuse = glm::vec4(red, green, blue, 1.f);
	theLights[lightNumber].specular = glm::vec4(red, green, blue, 1.f);
}

void cLightManager::SetPosition(unsigned int lightNumber, glm::vec3 newPosition) {
	theLights[lightNumber].position = glm::vec4(newPosition.x, newPosition.y, newPosition.z, 1.f);
}

void cLightManager::MoveLightRelative(unsigned int lightNumber, glm::vec3 deltaPosition) {

	//theLights[lightNumber].position += deltaPosition;
}


// This sets up the initial uniform locations from the shader
void cLightManager::SetUpUniformLocations(unsigned int shaderProgram)
{
	// GLint theLights_00_position = glGetUniformLocation(program, "theLights[0].position"); // vec4

//	struct sLight
//	{
//		vec4 position;
//		vec4 diffuse;
//		vec4 specular;	// rgb = highlight colour, w = power
//		vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
//		vec4 direction;	// Spot, directional lights
//		vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//						// 0 = pointlight
//						// 1 = spot light
//						// 2 = directional light
//		vec4 param2;	// x = 0 for off, 1 for on
//	};
//	uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms

	//this->theLights[0].position_uniform_location = 
	//					glGetUniformLocation(shaderProgram, "theLights[0].position"); 

	//this->theLights[0].diffuse_uniform_location = 
	//					glGetUniformLocation(shaderProgram, "theLights[0].diffuse");

	//this->theLights[0].specular_uniform_location =
	//					glGetUniformLocation(shaderProgram, "theLights[0].specular"); 

	//this->theLights[0].atten_uniform_location =
	//					glGetUniformLocation(shaderProgram, "theLights[0].atten"); 

	//this->theLights[0].direction_uniform_location =
	//					glGetUniformLocation(shaderProgram, "theLights[0].direction"); 

	//this->theLights[0].param1_uniform_location =
	//					glGetUniformLocation(shaderProgram, "theLights[0].param1"); 

	//this->theLights[0].param2_uniform_location =
	//					glGetUniformLocation(shaderProgram, "theLights[0].param2"); 

	//// And so on for the rest of the lights...
	//this->theLights[11].position_uniform_location =
	//	glGetUniformLocation(shaderProgram, "theLights[11].position");


	for (unsigned int lightIndex = 0; lightIndex != NUMBER_OF_LIGHTS; lightIndex++)
	{
		std::stringstream ssLight;
		ssLight << "theLights[" << lightIndex << "].";
		
		this->theLights[lightIndex].position_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "position").c_str() );

		this->theLights[lightIndex].diffuse_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "diffuse").c_str() );

		this->theLights[lightIndex].specular_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "specular").c_str() );

		this->theLights[lightIndex].atten_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "atten").c_str());

		this->theLights[lightIndex].direction_uniform_location =	glGetUniformLocation(shaderProgram, (ssLight.str() + "direction").c_str());

		this->theLights[lightIndex].param1_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "param1").c_str());

		this->theLights[lightIndex].param2_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "param2").c_str());

	}//for (unsigned int lightIndex

	return;
}

// Copies the values from the array into the shader
void cLightManager::CopyLightInfoToShader(void)
{
	for (unsigned int lightIndex = 0; lightIndex != NUMBER_OF_LIGHTS; lightIndex++) {

		glUniform4f(this->theLights[lightIndex].position_uniform_location,
			this->theLights[lightIndex].position.x,
			this->theLights[lightIndex].position.y,
			this->theLights[lightIndex].position.z,
			this->theLights[lightIndex].position.w);

		glUniform4f(this->theLights[lightIndex].diffuse_uniform_location,
			this->theLights[lightIndex].diffuse.x,
			this->theLights[lightIndex].diffuse.y,
			this->theLights[lightIndex].diffuse.z,
			this->theLights[lightIndex].diffuse.w);

		glUniform4f(this->theLights[lightIndex].specular_uniform_location,
			this->theLights[lightIndex].specular.x,
			this->theLights[lightIndex].specular.y,
			this->theLights[lightIndex].specular.z,
			this->theLights[lightIndex].specular.w);

		glUniform4f(this->theLights[lightIndex].atten_uniform_location,
			this->theLights[lightIndex].atten.x,
			this->theLights[lightIndex].atten.y,
			this->theLights[lightIndex].atten.z,
			this->theLights[lightIndex].atten.w);

		glUniform4f(this->theLights[lightIndex].direction_uniform_location,
			this->theLights[lightIndex].direction.x,
			this->theLights[lightIndex].direction.y,
			this->theLights[lightIndex].direction.z,
			this->theLights[lightIndex].direction.w);

		glUniform4f(this->theLights[lightIndex].param1_uniform_location,
			this->theLights[lightIndex].param1.x,
			this->theLights[lightIndex].param1.y,
			this->theLights[lightIndex].param1.z,
			this->theLights[lightIndex].param1.w);

		glUniform4f(this->theLights[lightIndex].param2_uniform_location,
			this->theLights[lightIndex].param2.x,
			this->theLights[lightIndex].param2.y,
			this->theLights[lightIndex].param2.z,
			this->theLights[lightIndex].param2.w);
	}


	return;
}
