#version 330 core
out vec4 FragColor;

in vec3 Normal; // 接下来，在片段着色器中定义相应的输入变量
in vec3 FragPos; // 最后，在片段着色器中添加相应的输入变量。
  
uniform vec3 lightPos; // 由于光源的位置是一个静态变量，我们可以简单地在片段着色器中把它声明为uniform
uniform vec3 viewPos; // 为了得到观察者的世界空间坐标，我们简单地使用摄像机对象的位置坐标代替（它当然就是观察者）。所以我们把另一个uniform添加到片段着色器，把相应的摄像机位置坐标传给片段着色器：
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient 环境光照(Ambient Lighting)：即使在黑暗的情况下，世界上通常也仍然有一些光亮（月亮、远处的光），所以物体几乎永远不会是完全黑暗的。为了模拟这个，我们会使用一个环境光照常量，它永远会给物体一些颜色。
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 漫反射光照(Diffuse Lighting)：模拟光源对物体的方向性影响(Directional Impact)。它是冯氏光照模型中视觉上最显著的分量。物体的某一部分越是正对着光源，它就会越亮。
    vec3 norm = normalize(Normal); // 我们同样希望确保所有相关向量最后都转换为单位向量，所以我们把法线和最终的方向向量都进行标准化：
    vec3 lightDir = normalize(lightPos - FragPos); // 光的方向向量是光源位置向量与片段位置向量之间的向量差。你可能记得在变换教程中，我们能够简单地通过让两个向量相减的方式计算向量差。
    float diff = max(dot(norm, lightDir), 0.0); // 我们使用max函数返回两个参数之间较大的参数，从而保证漫反射分量不会变成负数。
    vec3 diffuse = diff * lightColor;
    
    // specular 镜面光照(Specular Lighting)：模拟有光泽物体上面出现的亮点。镜面光照的颜色相比于物体的颜色会更倾向于光的颜色。
    float specularStrength = 0.5; // 首先，我们定义一个镜面强度(Specular Intensity)变量，给镜面高光一个中等亮度颜色，让它不要产生过度的影响。
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 我们先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。
    vec3 specular = specularStrength * spec * lightColor;  
        

	// 冯氏光照模型的主要结构由3个分量组成：环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照。
    vec3 result = (ambient + diffuse + specular) * objectColor; // 剩下的最后一件事情是把它加到环境光分量和漫反射分量里，再用结果乘以物体的颜色
    FragColor = vec4(result, 1.0);
} 