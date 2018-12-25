//#version 330

precision mediump int;
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


    float ratio=0.1;
    // ambient
    ambient = ratio * light.ambient * material.ambient;
    // diffuse
    norm = normalize(Normal);
    lightDir = normalize(light.position);
//    mediump vec3 lightDir = normalize(light.position - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse = ratio * light.diffuse * (diff * material.diffuse);
    // specular
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = ratio * light.specular * (spec * material.specular);
    result += ambient + diffuse + specular;

    // ambient
    ambient = ratio * light.ambient * material.ambient;
    // diffuse
    norm = normalize(Normal);
    lightDir = normalize(vec3(-1.0, -1.0, 1.0));
//    mediump vec3 lightDir = normalize(light.position - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse = ratio * light.diffuse * (diff * material.diffuse);
    // specular
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = ratio * light.specular * (spec * material.specular);
    result += ambient + diffuse + specular;

    gl_FragColor = vec4(result, 1.0);
}
