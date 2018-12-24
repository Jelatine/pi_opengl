//#version 330
//struct Material{
//    highp int en_diffuse_tex;
//    highp int en_specular_tex;
//    highp int en_emission_tex;
//    highp sampler2D tex_diffuse;
//    highp sampler2D tex_specular;
//    highp sampler2D tex_emission;
//    highp vec3 ambient;
//    highp vec3 diffuse;
//    highp vec3 specular;
//    highp float shininess;};
//struct Light{
//    highp vec3 position;
//    highp vec3 ambient;
//    highp vec3 diffuse;
//    highp vec3 specular;
//};
//varying highp vec3 FragPos;
//varying highp vec3 Normal;
//uniform highp vec3 viewPos;
//uniform  Material material;
//uniform  Light light;
//varying highp vec2 TexCoords;
////varying vec3 direction;
//highp vec3 CalcDirLight(Light _light, vec3 highp normal, vec3 highp _viewPos,vec3 highp _direction);
////vec3 CalcPointLight(Light _light, vec3 normal, vec3 fragPos, vec3 _viewPos);
//void main(){
//    highp vec3 result=vec3(0,0,0);
//    highp vec3 direction=vec3(-2,-2,-2);
//    result+=CalcDirLight(light,Normal,viewPos,direction);
////    direction=vec3(-2,2,-2);
////    result+=CalcDirLight(light,Normal,viewPos,direction);
////    direction=vec3(2,-2,2);
////    result+=CalcDirLight(light,Normal,viewPos,direction);
////    direction=vec3(-2,2,2);
////    result+=CalcDirLight(light,Normal,viewPos,direction);
//    gl_FragColor = vec4(result, 1.0);
//}


//highp vec3 CalcDirLight(Light _light, vec3 highp normal, vec3 highp _viewPos,vec3 highp _direction){
//    // ambient
//    highp vec3 ambient = _light.ambient * material.ambient;
//    // diffuse
//    highp vec3 norm = normalize(Normal);
//    highp vec3 lightDir = normalize(-_direction);
//    highp float diff = max(dot(norm, lightDir), 0.0);
//    highp vec3 diffuse = _light.diffuse * (diff * material.diffuse);
//    if(material.en_diffuse_tex>0)
//        diffuse = _light.diffuse * (diff * texture2D(material.tex_diffuse, TexCoords).rgb);
//    // specular
//    highp vec3 viewDir = normalize(_viewPos - FragPos);
//    highp vec3 reflectDir = reflect(-lightDir, norm);
//    highp float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    highp vec3 specular = _light.specular * (spec * material.specular);
//    if(material.en_specular_tex>0)
//        specular = _light.specular * (spec * texture2D(material.tex_specular, TexCoords).rgb);
//    highp vec3 emission=vec3(0.0,0.0,0.0);
//    if(material.en_emission_tex>0)
//        emission = texture2D(material.tex_emission, TexCoords).rgb;
//    return (ambient + diffuse + specular +emission);
//}



precision mediump int;
//precision mediump vec3;
//precision mediump vec4;
precision mediump float;
struct Material {
    mediump vec3 ambient;
    mediump vec3 diffuse;
    mediump vec3 specular;
    float shininess;
};

struct Light {
    mediump vec3 position;

    mediump vec3 ambient;
    mediump vec3 diffuse;
    mediump vec3 specular;
};
uniform mediump vec3 viewPos;
uniform Material material;
uniform Light light;

varying mediump vec3 FragPos;
varying mediump vec3 Normal;
void main(){
    // ambient
    mediump vec3 ambient = light.ambient * material.ambient;

    // diffuse
    mediump vec3 norm = normalize(Normal);
    mediump vec3 lightDir = normalize(-light.position);
//    mediump vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    mediump vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    mediump vec3 viewDir = normalize(viewPos - FragPos);
    mediump vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    mediump vec3 specular = light.specular * (spec * material.specular);

    mediump vec3 result = ambient + diffuse + specular;
    gl_FragColor = vec4(result, 1.0);
}
