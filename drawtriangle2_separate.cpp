

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;


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
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); // glfwCreateWindow������Ҫ���ڵĿ�͸���Ϊ����ǰ����������������������ʾ������ڵ����ƣ����⣩����������ʹ��"LearnOpenGL";
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


	Shader ourShader("shader.vs", "shader.fs"); // you can name your shader files however you like

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ��������ϣ����Ⱦһ�������Σ�����һ��Ҫָ���������㣬ÿ�����㶼��һ��3Dλ�á����ǻὫ�����Ա�׼���豸�������ʽ��OpenGL�Ŀɼ����򣩶���Ϊһ��float���顣
	float vertices_1[] = {
		-1.0f, -1.0f, 0.0f, // left  
		1.0f, -1.0f, 0.0f, // right 
		0.0f,  0.0f, 0.0f
	};

	float vertices_2[] = {
		-1.0f, 1.0f, 0.0f, // left  
		1.0f, 1.0f, 0.0f, // right 
		0.0f,  0.0f, 0.0f
	};

	// glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ��������ĵ�һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ��ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�����������������ϣ�����͵�ʵ�����ݡ�
	unsigned int VBO_1, VAO_1;
	glGenVertexArrays(1, &VAO_1); // ����һ��VAO_1�ʹ���һ��VBO_1������;
	glGenBuffers(1, &VBO_1); // ���㻺�������������OpenGL�̳��е�һ�����ֵ�OpenGL���󡣾���OpenGL�е���������һ�������������һ����һ�޶���ID���������ǿ���ʹ��glGenBuffers������һ������ID����һ��VBO����;
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_1); // Ҫ��ʹ��VAO_1��Ҫ����ֻ��ʹ��glBindVertexArray��VAO_1��

	glBindBuffer(GL_ARRAY_BUFFER, VBO_1); // OpenGL��������ͬʱ�󶨶�����壬ֻҪ�����ǲ�ͬ�Ļ������͡����ǿ���ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW); // ����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��Ȼ�����ǿ��Ե���glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // ������Щ��Ϣ���ǾͿ���ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ���;
	glEnableVertexAttribArray(0); // ���������Ѿ�������OpenGL����ν��Ͷ������ݣ���������Ӧ��ʹ��glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO_1 afterwards so other VAO_1 calls won't accidentally modify this VAO_1, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	//----------------------------------------------------------------------------------------------------
	unsigned int VBO_2, VAO_2;
	glGenVertexArrays(1, &VAO_2); // ����һ��VAO_1�ʹ���һ��VBO_1������;
	glGenBuffers(1, &VBO_2); // ���㻺�������������OpenGL�̳��е�һ�����ֵ�OpenGL���󡣾���OpenGL�е���������һ�������������һ����һ�޶���ID���������ǿ���ʹ��glGenBuffers������һ������ID����һ��VBO����;
							 // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_2); // Ҫ��ʹ��VAO_1��Ҫ����ֻ��ʹ��glBindVertexArray��VAO_1��

	glBindBuffer(GL_ARRAY_BUFFER, VBO_2); // OpenGL��������ͬʱ�󶨶�����壬ֻҪ�����ǲ�ͬ�Ļ������͡����ǿ���ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW); // ����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��Ȼ�����ǿ��Ե���glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // ������Щ��Ϣ���ǾͿ���ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ���;
	glEnableVertexAttribArray(0); // ���������Ѿ�������OpenGL����ν��Ͷ������ݣ���������Ӧ��ʹ��glEnableVertexAttribArray���Զ�������λ��ֵ��Ϊ���������ö������ԣ�

								  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO_1 afterwards so other VAO_1 calls won't accidentally modify this VAO_1, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//---------------------------------------------------------------------------------------------------
	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		// draw our first triangle
		ourShader.use(); // ���ǿ��Ե���glUseProgram�������øմ����ĳ��������Ϊ���Ĳ������Լ�������������;
		glBindVertexArray(VAO_1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3); // glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�����������һ��ʼʱ˵��������ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES�������ڶ�������ָ���˶����������ʼ����������������0�����һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3������ֻ�����ǵ���������Ⱦһ�������Σ���ֻ��3�����㳤����
		// glBindVertexArray(0); // no need to unbind it every time 

		// draw our first triangle
		ourShader.use(); // ���ǿ��Ե���glUseProgram�������øմ����ĳ��������Ϊ���Ĳ������Լ�������������;
		glBindVertexArray(VAO_2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3); // glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�����������һ��ʼʱ˵��������ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES�������ڶ�������ָ���˶����������ʼ����������������0�����һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3������ֻ�����ǵ���������Ⱦһ�������Σ���ֻ��3�����㳤����
		// glBindVertexArray(0); // no need to unbind it every time 


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwPollEvents(); // glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_1);
	glDeleteBuffers(1, &VBO_1);

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
// Ȼ�������û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��������ÿ�δ��ڴ�С��������ʱ�򱻵��á�
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height); // �����ǿ�ʼ��Ⱦ֮ǰ����һ����Ҫ������Ҫ�������Ǳ������OpenGL��Ⱦ���ڵĳߴ��С�����ӿ�(Viewport)������OpenGL��ֻ��֪���������ݴ��ڴ�С��ʾ���ݺ����ꡣ���ǿ���ͨ������glViewport���������ô��ڵ�ά��(Dimension);
}

