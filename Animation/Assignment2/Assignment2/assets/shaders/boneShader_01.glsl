// Vertex shader
#version 420

//uniform mat4 MVP;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModelInverseTranspose;	// For normal calculation

//uniform bool bUseVertexColour;		// Will default to GL_FALSE, which is zero (0)
//uniform vec3 vertexColourOverride;

in vec4 vColour;
in vec4 vPosition;
in vec4 vNormal;				// Vertex normal X,Y,Z (W ignored)
in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
in vec4 vBoneWeights;
in vec4 vBoneIDs;


out vec4 fVertexColour;			// used to be "out vec3 color"
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;

uniform mat4 BoneMatrices[4];
uniform bool bUseBones;

void main()
{
	// Order of these is important
	//mvp = p * v * matModel; from C++ code
	
	mat4 MVP = matProjection * matView * matModel;
			
	//Get the bone position.
	vec4 position = vec4(0.f, 0.f, 0.f, 0.f);
	if (bUseBones){
		position = BoneMatrices[int(vBoneIDs[0])] * vPosition;
	}
	else {
		position = vPosition;
	}
	
    gl_Position = MVP * position; 		// Used to be: vec4(vPosition, 1.0f);	// Used to be vPos
	
	// The location of the vertex in "world" space (not screen space)
	fVertWorldLocation = matModel * position;
	
	// Copy the vertex colour to the fragment shader
	// (if you want the colours from the original file used)
    fVertexColour = vColour;		// Used to be vCol
	
	// Calculate the normal based on any rotation we've applied.
	// This inverse transpose removes scaling and tranlation (movement) 
	// 	from the matrix.
	fNormal = matModelInverseTranspose * normalize(vNormal);
	fNormal = normalize(fNormal);
	
	// Copy the rest of the vertex values:
	fUVx2 = vUVx2;
};
