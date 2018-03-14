

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // λ�ñ���������λ��ֵΪ 0 
"layout (location = 1) in vec3 aColor;\n" // ��ɫ����������λ��ֵΪ 1
"out vec3 ourColor;\n" // ���ǿ�����out�ؼ��������������;
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n" // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" // ���ǿ�����out�ؼ����������������������������ΪFragColor��
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit(); // ������main�����е���glfwInit��������ʼ��GLFW;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ���ǽ����汾��(Major)�ʹΰ汾��(Minor)����Ϊ3;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ���ǽ����汾��(Major)�ʹΰ汾��(Minor)����Ϊ3;
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ����ͬ����ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
    // glfwCreateWindow������Ҫ���ڵĿ�͸���Ϊ����ǰ����������������������ʾ������ڵ����ƣ����⣩����������ʹ��"LearnOpenGL";
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // ����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ�����ǿ�����main������������glfwTerminate��������ɡ�
		return -1;
	}
	glfwMakeContextCurrent(window); // �����괰�����ǾͿ���֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳�����������;
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ���ǻ���Ҫע���������������GLFW����ϣ��ÿ�����ڵ�����С��ʱ������������;

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER); // ��������Ҫ�����Ǵ���һ����ɫ������ע�⻹����ID�����õġ��������Ǵ������������ɫ��Ϊunsigned int��Ȼ����glCreateShader���������ɫ��;
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // glShaderSource������Ҫ�������ɫ��������Ϊ��һ���������ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ���������������Ƕ�����ɫ��������Դ�룬���ĸ���������������ΪNULL��
	glCompileShader(vertexShader); // ��һ�����ǰ������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ�������;
	// check for shader compile errors
	int success; // �������Ƕ���һ�����ͱ�������ʾ�Ƿ�ɹ�����;
	char infoLog[512]; // ��������һ�����������Ϣ������еĻ�����������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Ȼ��������glGetShaderiv����Ƿ����ɹ���
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // �������ʧ�ܣ����ǻ���glGetShaderInfoLog��ȡ������Ϣ��Ȼ���ӡ����
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // ����Ƭ����ɫ���Ĺ����붥����ɫ�����ƣ�ֻ��������ʹ��GL_FRAGMENT_SHADER������Ϊ��ɫ������;
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // �������ʧ�ܣ����ǻ���glGetShaderInfoLog��ȡ������Ϣ��Ȼ���ӡ����
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram(); // glCreateProgram��������һ�����򣬲������´�����������ID���á�
	glAttachShader(shaderProgram, vertexShader); // ����������Ҫ��֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram��������;
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // ��glLinkProgram��������;
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // ������ɫ���ı���һ��������Ҳ���Լ��������ɫ�������Ƿ�ʧ�ܣ�����ȡ��Ӧ����־��
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); // �ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������;
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); // ����һ��VAO�ʹ���һ��VBO������;
	glGenBuffers(1, &VBO); // ���㻺�������������OpenGL�̳��е�һ�����ֵ�OpenGL���󡣾���OpenGL�е���������һ�������������һ����һ�޶���ID���������ǿ���ʹ��glGenBuffers������һ������ID����һ��VBO����;
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO); // Ҫ��ʹ��VAO��Ҫ����ֻ��ʹ��glBindVertexArray��VAO��

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // OpenGL��������ͬʱ�󶨶�����壬ֻҪ�����ǲ�ͬ�Ļ������͡����ǿ���ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��Ȼ�����ǿ��Ե���glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // ������Щ��Ϣ���ǾͿ���ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ���;
	glEnableVertexAttribArray(0); // ���������Ѿ�������OpenGL����ν��Ͷ������ݣ���������Ӧ��ʹ��glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // ��ɫ���Խ���λ������֮������ƫ��������3 * sizeof(float)�����ֽ����������12�ֽڡ�
	glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	// as we only have a single shader, we could also just activate our shader once beforehand if we want to 
	glUseProgram(shaderProgram); // ���ǿ��Ե���glUseProgram�������øմ����ĳ��������Ϊ���Ĳ������Լ�������������;

	// render loop
	// ������Ҫ�ڳ��������һ��whileѭ�������ǿ��԰�����֮Ϊ��Ⱦѭ��(Render Loop)��������������GLFW�˳�ǰһֱ�������С�
	while (!glfwWindowShouldClose(window)) // glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	{
		// input
		// -----
		processInput(window); // ���ǽ���������Ⱦѭ����ÿһ�������е���processInput;

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ����glClear֮�⣬���ǻ�������glClearColor�����������Ļ���õ���ɫ��������glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ����������ǽ���Ļ����Ϊ�����ƺڰ��������ɫ��
		glClear(GL_COLOR_BUFFER_BIT); // ���ǿ���ͨ������glClear�����������Ļ����ɫ���壬������һ������λ(Buffer Bit)��ָ��Ҫ��յĻ��壬���ܵĻ���λ��GL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT��������������ֻ������ɫֵ����������ֻ�����ɫ���塣

		// render the triangle
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3); // glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�����������һ��ʼʱ˵��������ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES�������ڶ�������ָ���˶����������ʼ����������������0�����һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3������ֻ�����ǵ���������Ⱦһ�������Σ���ֻ��3�����㳤����

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwPollEvents(); // glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate(); // ����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ�����ǿ�����main������������glfwTerminate��������ɡ�
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // ����ͬ��Ҳϣ���ܹ���GLFW��ʵ��һЩ������ƣ������ͨ��ʹ��GLFW�ļ������뺯������ɡ����ǽ���ʹ��GLFW��glfwGetKey����������Ҫһ�������Լ�һ��������Ϊ���롣�������Ǽ���û��Ƿ����˷��ؼ�(Esc)�����û�а��£�glfwGetKey���᷵��GLFW_RELEASE������û���ȷ�����˷��ؼ������ǽ�ͨ��glfwSetwindowShouldCloseʹ�ð�WindowShouldClose��������Ϊ true�ķ����ر�GLFW����һ��whileѭ����������⽫��ʧ�ܣ����򽫻�رա�
		glfwSetWindowShouldClose(window, true); // ����������᷵����������Ƿ����ڱ����¡����ǽ�����һ��processInput�����������е�������뱣�����ࡣ
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height); // �����ǿ�ʼ��Ⱦ֮ǰ����һ����Ҫ������Ҫ�������Ǳ������OpenGL��Ⱦ���ڵĳߴ��С�����ӿ�(Viewport)������OpenGL��ֻ��֪���������ݴ��ڴ�С��ʾ���ݺ����ꡣ���ǿ���ͨ������glViewport���������ô��ڵ�ά��(Dimension);
}

