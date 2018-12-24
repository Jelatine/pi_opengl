//#version 330
//precision mediump vec2;
//precision mediump vec3;
//precision mediump mat3;
//precision mediump mat4;
attribute mediump vec3 aPos;
attribute mediump vec3 aNormal;
attribute mediump vec2 aTexCoords;
varying mediump vec3 Normal;
varying mediump vec3 FragPos;
varying mediump vec2 TexCoords;
uniform mediump mat4 model;
uniform mediump mat3 invmodel;
uniform mediump mat4 view;
uniform mediump mat4 projection;
void main(){
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal=   invmodel* aNormal;;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
