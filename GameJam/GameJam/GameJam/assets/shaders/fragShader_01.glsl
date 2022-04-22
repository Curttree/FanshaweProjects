// Fragment shader
#version 420

in vec4 fVertexColour;			// The vertex colour from the original model
in vec4 fVertWorldLocation;
in vec4 fNormal;
in vec4 fUVx2;
in vec4 fDebugColourOverride;	// If debug normals are being drawn, this is the colour

// Replaces gl_FragColor
out vec4 pixelOutputFragColour;			// RGB Alpha   (0 to 1) 
out vec4 pixelOutputMaterialColour;		// = 1;
out vec4 pixelOutputNormal;				// = 2;
out vec4 pixelOutputWorldPos;			// = 3;
out vec4 pixelOutputSpecular;			// = 4;

// The "whole object" colour (diffuse and specular)
uniform vec4 wholeObjectDiffuseColour;	// Whole object diffuse colour
uniform bool bUseWholeObjectDiffuseColour;	// If true, the whole object colour is used (instead of vertex colour)
uniform vec4 wholeObjectSpecularColour;	// Colour of the specular highlight (optional)

// Alpha transparency value
uniform float wholeObjectAlphaTransparency;

// This is used for wireframe or whole object colour. 
// If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
uniform bool bUseDebugColour;	
uniform vec4 objectDebugColour;		
// If this is true, then the vertex is drawn with no lighting and this colour	
uniform bool bDrawDebugNormals;			// Default is 0 (or FALSE)

// This will not modulate the colour by the lighting contribution.
// i.e. shows object colour "as is". 
// Used for wireframe or debug type objects
uniform bool bDontLightObject;			// 1 if you want to AVOID lighting

// for the imposter models
uniform bool bIsImposter;

// This is the camera eye location (update every frame)
uniform vec4 eyeLocation;

uniform vec2 screenWidthHeight;

// Indicates which 'pass' we are doing
const uint PASS_0_ENTIRE_SCENE = 0;
const uint PASS_1_QUAD_ONLY = 1;
const uint PASS_2_MONITOR = 2;
const uint PASS_3_2D_EFFECTS_PASS = 3;
const uint PASS_4_UI_PASS = 3;
uniform uint renderPassNumber;


struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;


const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms
// 
// uniform vec4 theLights[0].position;
// uniform vec4 theLights[0].diffuse;
// ...
// uniform vec4 theLights[1].position;
// uniform vec4 theLights[1].diffuse;
// ...

vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );
							
uniform sampler2D texture_00;		// GL_TEXTURE_2D
uniform sampler2D texture_01;		// GL_TEXTURE_2D
uniform sampler2D texture_02;		// GL_TEXTURE_2D
uniform sampler2D texture_03;		// GL_TEXTURE_2D
uniform sampler2D texture_04;		// GL_TEXTURE_2D
uniform sampler2D texture_05;		// GL_TEXTURE_2D
uniform sampler2D texture_06;		// GL_TEXTURE_2D
uniform sampler2D texture_07;		// GL_TEXTURE_2D
uniform sampler2D texture_08;		// GL_TEXTURE_2D
uniform sampler2D texture_09;		// GL_TEXTURE_2D

uniform bool bShowCrosshair;
uniform sampler2D crosshair;		// GL_TEXTURE_2D

uniform vec4 texture2D_Ratios0to3;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );
uniform vec4 texture2D_Ratios4to7;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );

// Cube maps for skybox, etc.
uniform samplerCube cubeMap_00;			// Tropical day time
uniform samplerCube cubeMap_01;			// Tropical night time
uniform samplerCube cubeMap_02;
uniform samplerCube cubeMap_03;
uniform vec4 cubeMap_Ratios0to3;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );

// Mask textures
uniform sampler2D maskTexture_00;		// GL_TEXTURE_2D
uniform sampler2D maskTexture_01;		// GL_TEXTURE_2D
// Textures to use with masks
uniform sampler2D maskTextureTop_00;		// GL_TEXTURE_2D
uniform sampler2D maskTextureTop_01;		// GL_TEXTURE_2D
uniform vec4 mask_Ratios0to1;

// if true, then we only sample from the cubeMaps (skyboxes)
uniform bool bIsSkyBox;

// If this is true, then we will sample the "discardTexture" to 
//	perform a discard on that pixel
// (we could also do a change in the transparency, or something)
uniform sampler2D discardTexture;
uniform vec3 discardColour;		
uniform bool bDiscardTransparencyWindowsOn;

//Reflection and Refraction
uniform bool bDoesReflect;
uniform bool bDoesRefract;
uniform float fRefractionIndex;


// Specular map values. Decide whether you want to use a map, or whole object spec value (default)
uniform bool bUseSpecularMap;	
uniform sampler2D specularMapTexture;	

// Effect textures
uniform sampler2D staticTexture;
uniform sampler2D waterTexture;

uniform vec3 maxDepthColour;
uniform vec3 depthColourMultiplier;

uniform float deltaTime;
uniform float totalTime;

void main()
{
	// This is the pixel colour on the screen.
	// Just ONE pixel, though.
	
	vec3 maxDepthColour = vec3(1.0f, 0.f, 0.f);
	vec3 depthColourMultiplier = vec3(1.0f, 1.0f, 2.5f);
	float maxDepth = 100.f;
	float minDepth = 30.f;
	
	// HACK: See if the UV coordinates are actually being passed in
	pixelOutputFragColour.rgba = vec4(0.0f, 0.0f, 0.0, 1.0f); 
	
	if ( renderPassNumber == PASS_1_QUAD_ONLY )
	{
		// Render the texture to the quad, and that's it
		vec2 UVlookup;
		UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
		UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
		vec3 sampleColour = texture( texture_07, UVlookup ).rgb;

		pixelOutputFragColour.rgb = sampleColour.rgb;
		pixelOutputFragColour.a = 1.0f;
		return;
	}

	if (renderPassNumber == PASS_2_MONITOR)
	{
		//vec3 sampleColour = texture(texture_08, fUVx2.xy).rgb;
		//sampleColour.rgb *= texture(staticTexture, fUVx2.xy).rgb;
		//pixelOutputFragColour.rgb = sampleColour.rgb;
		//pixelOutputFragColour.a = 1.0f;
		return;
	}

	if (renderPassNumber == PASS_3_2D_EFFECTS_PASS)
	{
		// Render the texture to the quad, and add in effects.

		vec2 UVlookup;
		UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
		UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height

		//Let's try the water effect.
		vec2 timeUV;
		timeUV.x = mod((UVlookup.x + totalTime),1.0f);
		timeUV.y = 0.f;
		vec4 bumpTex = texture( waterTexture, timeUV);
		vec2 normal = bumpTex.xy * 2.f - 1.f;
		UVlookup += normal * 0.0025f;
		vec3 sampleColour = texture(texture_07, UVlookup).rgb;
		
		vec3 blurColour = sampleColour;
		for (float i = 1; i < 6; i++)
		{
			float offset = i * 0.002f;
			vec2 UVlookup;
			UVlookup.x = gl_FragCoord.x / screenWidthHeight.x+offset;	// Width
			UVlookup.y = gl_FragCoord.y / screenWidthHeight.y+offset;	// Height
			blurColour.r += texture(texture_07, UVlookup).r;
			blurColour.g += texture(texture_07, UVlookup).g;
			blurColour.b += texture(texture_07, UVlookup).b;
		}
		for (float i = -1; i > -6; i--)
		{
			float offset = i * 0.002f;
			vec2 UVlookup;
			UVlookup.x = gl_FragCoord.x / screenWidthHeight.x + offset;	// Width
			UVlookup.y = gl_FragCoord.y / screenWidthHeight.y + offset;	// Height
			blurColour.r += texture(texture_07, UVlookup).r;
			blurColour.g += texture(texture_07, UVlookup).g;
			blurColour.b += texture(texture_07, UVlookup).b;
		}
		blurColour /= 11.f;
		//For a funky effect, enable this line.
		//sampleColour += (blurColour - sampleColour) * texture(texture_07, UVlookup).r * 10.f;
		if (texture(texture_08, UVlookup).r  > 0.5f){
		
		sampleColour += (blurColour - sampleColour) * (texture(texture_08, UVlookup).r - 0.5f);
		}
		
		if (totalTime < 4.2f){
		sampleColour.rgb *= totalTime / 4.2f;
		}

		// Tint the colour blue.
		sampleColour.b *= 1.25f;
		
		if (bShowCrosshair){
			vec2 UVlookupCrosshair;
			UVlookupCrosshair.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
			UVlookupCrosshair.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
			sampleColour.r += texture(crosshair, UVlookupCrosshair).r;
			sampleColour.g += texture(crosshair, UVlookupCrosshair).g;
			sampleColour.b += texture(crosshair, UVlookupCrosshair).b;
		}
		
		pixelOutputFragColour.rgb = sampleColour.rgb;
		//Does this need to be 100%? TODO: See if I can turn down alpha.
		pixelOutputFragColour.a = 1.0f;
		
		return;
	}
	if (renderPassNumber == PASS_4_UI_PASS){
		// Don't do lighting, but do respect discard transparency.
		//vec4 vertexDiffuseColour = fVertexColour;
		//vertexDiffuseColour.rgb *= 0.0001f;
		pixelOutputFragColour.rgba = vec4(0.0f, 0.0f, 0.0, 1.0f); 
		//pixelOutputFragColour.a = wholeObjectAlphaTransparency;
		
		vec2 UVlookup;
		UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
		UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
		vec3 sampleColour = texture(texture_07, UVlookup).rgb;
		
		pixelOutputFragColour.rgb = normalize(sampleColour);
		return;
	}

	pixelOutputFragColour.a = wholeObjectAlphaTransparency;
	
	// If face normals are being generated from the geometry shader, 
	//	then this is true, and the colours are taken from the 
	if ( int(fDebugColourOverride.w) == 1 )
	{
		pixelOutputFragColour = fDebugColourOverride;
		return;	
	}
	
	// Perform a discard transparency action for the "windows"
	if (bDiscardTransparencyWindowsOn)
	{
		// Eventually I may want to make this configurable.
		vec3 vec3DisSample = texture( discardTexture, fUVx2.xy ).rgb;
		// Take average of this RGB sample
		//
		if (abs(discardColour.r - vec3DisSample.r) < 0.7f &&  abs(discardColour.g - vec3DisSample.g) < 0.7f && abs(discardColour.b - vec3DisSample.b) < 0.7f)
		{	// "close enough"
		
			// DON'T even draw the pixel here
			// The fragment shader simply stops here
			discard;
		}
		else{
		pixelOutputFragColour = vec4(vec3DisSample.x,vec3DisSample.y,vec3DisSample.z,wholeObjectAlphaTransparency);
		}
		return;
	}// if (bDiscardTransWindowsOn)
	
	if ( bIsSkyBox )
	{
		// For some reason if the cube map isn't actaully set to 
		//	a texture unit, it returns black when combined with other cubemaps
		//		pixelColour.rgb = 
		//			( texture( cubeMap_00, fNormal.xyz ).rgb * cubeMap_Ratios0to3.x ) + 
		//			( texture( cubeMap_01, fNormal.xyz ).rgb * cubeMap_Ratios0to3.y ) +
		//			( texture( cubeMap_02, fNormal.xyz ).rgb * cubeMap_Ratios0to3.z ) + 
		//			( texture( cubeMap_03, fNormal.xyz ).rgb * cubeMap_Ratios0to3.w );	
		//
		//	So here's an alternative work around version:
		//
		pixelOutputFragColour.rgba = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		if ( cubeMap_Ratios0to3.x > 0.0f )
		{
			pixelOutputFragColour.rgb += texture( cubeMap_00, fNormal.xyz ).rgb * cubeMap_Ratios0to3.x;
		}

		return;	
	}//if ( bIsSkyBox )
	
	if ( bDoesReflect )
	{
		vec3 reflectVector = reflect((fVertWorldLocation.xyz - eyeLocation.xyz), fNormal.xyz);
		pixelOutputFragColour.rgba = vec4(0.0f, 0.0f, 0.0f, wholeObjectAlphaTransparency);

		if ( cubeMap_Ratios0to3.x > 0.0f )
		{
			pixelOutputFragColour.rgb += texture( cubeMap_00, vec3(reflectVector.x, -reflectVector.y, reflectVector.z)).rgb;
		}
		if (bUseWholeObjectDiffuseColour) {
			pixelOutputFragColour.rgb *= wholeObjectDiffuseColour.rgb;
		}

		return;	
	}//if ( bDoesReflect )
	
	if ( bDoesRefract )
	{
		vec3 refractVector = refract((eyeLocation.xyz - fVertWorldLocation.xyz), fNormal.xyz, fRefractionIndex);
		pixelOutputFragColour.rgba = vec4(0.0f, 0.0f, 0.0f, wholeObjectAlphaTransparency);

		if ( cubeMap_Ratios0to3.x > 0.0f )
		{
			pixelOutputFragColour.rgb += texture( cubeMap_00, vec3(refractVector.x,refractVector.y,refractVector.z) ).rgb;
		}

		return;	
	}//if ( bDoesRefract )
	
	// Copy model vertex colours?
	vec4 vertexDiffuseColour = fVertexColour;
	
	// Use model vertex colours or not?
	if ( bUseWholeObjectDiffuseColour )
	{
		vertexDiffuseColour = wholeObjectDiffuseColour;
	}
	
	// Use debug colour?
	if ( bUseDebugColour )
	{
		// Overwrite the vertex colour with this debug colour
		vertexDiffuseColour = objectDebugColour;	
	}
	
	if ( (bUseWholeObjectDiffuseColour == false) && 
	     (bUseDebugColour == false) )
	{
		vertexDiffuseColour.rgb = 	
				(texture( texture_00, fUVx2.xy ).rgb * texture2D_Ratios0to3.x)  + 
				(texture( texture_01, fUVx2.xy ).rgb * texture2D_Ratios0to3.y)  + 
				(texture( texture_02, fUVx2.xy ).rgb * texture2D_Ratios0to3.z)  + 
				(texture( texture_03, fUVx2.xy ).rgb * texture2D_Ratios0to3.w);
			// + etc... the other 4 texture units
	}

	// Used for drawing "debug" objects (usually wireframe)
	if ( bDontLightObject )
	{
		pixelOutputFragColour.rgb= vertexDiffuseColour.rgb;
		pixelOutputFragColour.a = wholeObjectAlphaTransparency;		
		// Early exit from shader
		return;
	}

	// Makes this "black" but not quite...
	vertexDiffuseColour.rgb *= 0.0001f;

	vertexDiffuseColour.rgb += 	
			(texture( texture_00, fUVx2.xy ).rgb * texture2D_Ratios0to3.x)  + 
		    (texture( texture_01, fUVx2.xy ).rgb * texture2D_Ratios0to3.y)  + 
			(texture( texture_02, fUVx2.xy ).rgb * texture2D_Ratios0to3.z)  + 
			(texture( texture_03, fUVx2.xy ).rgb * texture2D_Ratios0to3.w);
			// + etc... the other 4 texture units
			
	
	vertexDiffuseColour.rgb += 	
			(texture( maskTexture_00, fUVx2.xy ).rgb * texture( maskTextureTop_00, fUVx2.xy ).rgb * mask_Ratios0to1.x)  + 
		    (texture( maskTexture_01, fUVx2.xy ).rgb * texture( maskTextureTop_01, fUVx2.xy ).rgb * mask_Ratios0to1.y);
			// 		
	
	vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,		
	                                        fNormal.xyz, 		// Normal at the vertex (in world coords)
                                            fVertWorldLocation.xyz,	// Vertex WORLD position
											wholeObjectSpecularColour.rgba );
											
	pixelOutputFragColour = outColour;
	pixelOutputFragColour.a = wholeObjectAlphaTransparency;
	
	if (length(fVertWorldLocation - eyeLocation) > maxDepth){
		pixelOutputFragColour.r *= depthColourMultiplier.r;
		pixelOutputFragColour.g *= depthColourMultiplier.g;
		pixelOutputFragColour.b *= depthColourMultiplier.b;
	}
	else if (length(fVertWorldLocation - eyeLocation) > minDepth && length(fVertWorldLocation - eyeLocation)/maxDepth * depthColourMultiplier.b >1.f){
		pixelOutputFragColour.b *= length(fVertWorldLocation - eyeLocation)/maxDepth * depthColourMultiplier.b;
	}
	
	// Output the other things for the G-Buffer:
	pixelOutputNormal = vec4(fNormal.xyz, 1.0f);
	pixelOutputMaterialColour = vec4(vertexDiffuseColour.rgb, 1.0f);
	pixelOutputWorldPos = vec4(fVertWorldLocation.xyz, 1.0f);
	pixelOutputSpecular = wholeObjectSpecularColour.rgba;			// = 4;	
};



// Calculates the colour of the vertex based on the lighting and vertex information:
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.25f, dotProduct );		
		
			lightContrib *= dotProduct;		
			lightContrib *= theLights[index].diffuse.a;
//			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
			finalObjectColour.rgb += (vertexMaterialColour.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)
			
			continue;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		
				
		float objectSpecularPower = vertexSpecular.w; 
		
		
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
			
		
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	

		if( bUseSpecularMap ) {
			attenuation *= texture( specularMapTexture, fUVx2.xy ).r;
			// 	objectSpecularPower *= 0.f;
		}		
		
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		lightDiffuseContrib *= theLights[index].diffuse.a;
		lightSpecularContrib *= theLights[index].diffuse.a;
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
			
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}//for(intindex=0...
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}
