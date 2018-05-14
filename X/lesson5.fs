#version 330 core

in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){
     vec3 specular = texture( myTextureSampler, UV ).rgb;
    // Output color = color of the texture at the specified UV
    vec4 temp = vec4(specular,0.0);
    temp.a = 0.5;
    color = temp;
}