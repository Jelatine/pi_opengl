//#version 330 core
attribute mediump vec3 aPos;

varying mediump vec3 TexCoords;

uniform mediump mat4 projection;
uniform mediump mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
