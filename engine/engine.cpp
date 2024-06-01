#define STB_IMAGE_IMPLEMENTATION
#include "engine.h"

namespace Engine {
	// Window part

	WindowCreateInfo::WindowCreateInfo() {
		this->width = 960;
		this->height = 540;
		this->title = "Sample title";
		
		this->resizable = true;
		this->vSyncEnabled = true;
	}

	GLFWwindow *Window::handle = nullptr;
	bool Window::running = false, Window::created = false;

	int Window::x = 0;
	int Window::y = 0;

	int Window::width = 0;
	int Window::height = 0;

	void Window::onResize(GLFWwindow* handle, int width, int height) {
		Window::width = width;
		Window::height = height;

		glViewport(0, 0, width, height);
	}
	void Window::onMove(GLFWwindow* handle, int x, int y) {
		Window::x = x;
		Window::y = y;
	}

	void Window::create(const WindowCreateInfo &createInfo) {
        Logger::ClearFromFile();
		if (Window::created) {
            Logger::Log(Logger::WARNING, "TT::Window::create (const WindowCreateInfo &createInfo): Window already created!");
			return;
		}

		Window::created = true;

		if (!glfwInit()) {
            Logger::Log(Logger::ERROR, "TT::Window::create (const WindowCreateInfo &createInfo): Could not initialize GLFW.");
			exit(1);
		}

		Window::width = createInfo.width;
		Window::height = createInfo.height;
		Window::running = true;

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RESIZABLE, createInfo.resizable);

		Window::handle = glfwCreateWindow(createInfo.width, createInfo.height, createInfo.title.c_str(), NULL, NULL);
		if (!Window::handle) {
			Window::running = false;

            Logger::Log(Logger::ERROR, "TT::Window::create (const WindowCreateInfo &createInfo): Window not created (Unknown Error)");

			glfwTerminate();
			exit(1);
		}

		glfwSetKeyCallback(Window::handle, [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS) {
				Keyboard::onKeyPress((uint16_t)key);
			}
			else if (action == GLFW_RELEASE) {
				Keyboard::onKeyRelease((uint16_t)key);
			}
		});
		glfwSetMouseButtonCallback(Window::handle, [](GLFWwindow* handle, int button, int action, int mods) {
			if (action == GLFW_PRESS) {
				Mouse::onButtonPress((uint8_t)button);
			}
			else if (action == GLFW_RELEASE) {
				Mouse::onButtonRelease((uint8_t)button);
			}
		});

		glfwSetWindowSizeCallback(Window::handle, Window::onResize);
		glfwSetWindowPosCallback(Window::handle, Window::onMove);

		glfwMakeContextCurrent(Window::handle);
		glfwSwapInterval(createInfo.vSyncEnabled);
		
		if (!gladLoadGL()) {
            Logger::Log(Logger::ERROR, "TT::Window::create (const WindowCreateInfo &createInfo): Glad not loaded...");
			exit(1);
		}

		stbi_set_flip_vertically_on_load(true);
        Logger::Log(Logger::INFO, "TT::Window::create (const WindowCreateInfo &createInfo): Successfully");
	}
	void Window::pollEvents() {
		glfwPollEvents();

		int width, height;
		glfwGetWindowSize(Window::handle, &width, &height);

		Window::width = width;
		Window::height = height;

		Window::running = !glfwWindowShouldClose(Window::handle);

		Keyboard::update();
		Mouse::update();
	}
	void Window::swapBuffers() {
		glfwSwapBuffers(Window::handle);
	}

	void Window::close() {
		Window::running = false;
		Window::created = false;

        Logger::Log(Logger::INFO, "TT::Window::close: Window closed!");

		glfwDestroyWindow(Window::handle);
		glfwTerminate();
	}

	void Window::setSize(const int width, const int height) {
		glfwSetWindowSize(Window::handle, width, height);
		Window::onResize(Window::handle, width, height);
        Logger::Log(Logger::INFO, "TT::Window::setSize (const int width, const int height): Window sized!");
	}
	void Window::setWidth(const int width) {
		Window::setSize(width, Window::height);
        Logger::Log(Logger::INFO, "TT::Window::setWidth (const int width): Window sized!");
	}
	void Window::setHeight(const int height) {
		Window::setSize(Window::width, height);
        Logger::Log(Logger::INFO, "TT::Window::setHeight (const int height): Window sized!");
	}

	void Window::lockAspect(const int x, const int y) {
		glfwSetWindowAspectRatio(Window::getHandle(), x, y);
        Logger::Log(Logger::INFO, "TT::Window::lockAspect (const int x, const int y): locked aspect!");
	}
	void Window::unlockAspect() {
		glfwSetWindowAspectRatio(Window::getHandle(), GLFW_DONT_CARE, GLFW_DONT_CARE);
        Logger::Log(Logger::INFO, "TT::Window::unlockAspect: unlocked aspect!");
	}

	void Window::setPosition(const int x, const int y) {
		glfwSetWindowPos(Window::handle, x, y);
		Window::onMove(Window::handle, x, y);
        Logger::Log(Logger::INFO, "TT::Window::setPosition (const int x, const int y): set x/y position!");
	}
	void Window::setX(const int x) {
		Window::setPosition(x, Window::y);
        Logger::Log(Logger::INFO, "TT::Window::setX (const int x): set position x!");
	}
	void Window::setY(const int y) {
		Window::setPosition(Window::x, y);
        Logger::Log(Logger::INFO, "TT::Window::setY (const int y): set position y!");
	}

	void Window::setTitle(const std::string& title) {
		glfwSetWindowTitle(Window::handle, title.c_str());
        Logger::Log(Logger::INFO, "TT::Window::setTitle: Changed title!");
	}

	GLFWwindow* Window::getHandle() {
		return Window::handle;
	}

	bool Window::isRunning() {
		return Window::running;
	}
	int Window::getWidth() {
		return Window::width;
	}
	int Window::getHeight() {
		return Window::height;
	}

	glm::ivec2 Window::getSize() {
		return glm::ivec2(Window::width, Window::height);
	}

	int Window::getX() {
		return Window::x;
	}
	int Window::getY() {
		return Window::y;
	}

	glm::ivec2 Window::getPosition() {
		return glm::ivec2(Window::x, Window::y);
	}

	// Keyboard part

	std::unordered_map<uint16_t, uint64_t> Keyboard::keys = {};
	uint64_t Keyboard::currentFrame = 1;

	void Keyboard::update() {
		Keyboard::currentFrame++;
	}
	
	void Keyboard::onKeyPress(const uint16_t key) {
		Keyboard::keys[key] = currentFrame;
	}
	void Keyboard::onKeyRelease(const uint16_t key) {
		Keyboard::keys[key] = 0;
	}

	bool Keyboard::isKeyPressed(const uint16_t key) {
		return Keyboard::keys[key] != 0;
	}
	bool Keyboard::isKeyJustPressed(const uint16_t key) {
		return Keyboard::keys[key] == currentFrame - 1;
	}

	// Mouse part

	uint64_t Mouse::buttons[] = {};
	uint64_t Mouse::currentFrame = 1;

	glm::vec2 Mouse::lastPosition = {}, Mouse::velocity = {};

	void Mouse::update() {
		glm::vec2 position = Mouse::getPosition();
		
		Mouse::velocity = glm::vec2(position - Mouse::lastPosition);
		Mouse::lastPosition = glm::vec2(position);

		Mouse::currentFrame++;
	}

	void Mouse::onButtonPress(const uint8_t button) {
		Mouse::buttons[button] = Mouse::currentFrame;
	}
	void Mouse::onButtonRelease(const uint8_t button) {
		Mouse::buttons[button] = 0;
	}

	bool Mouse::isButtonPressed(const uint8_t button) {
		return Mouse::buttons[button] != 0;
	}
	bool Mouse::isButtonJustPressed(const uint8_t button) {
		return Mouse::buttons[button] == Mouse::currentFrame - 1;
	}

	glm::vec2 Mouse::getPosition() {
		double x, y;
		glfwGetCursorPos(Window::getHandle(), &x, &y);

		return glm::vec2((float)x, (float)y);
	}
	glm::vec2 Mouse::getVelocity() {
		return Mouse::velocity;
	}

	void Mouse::setGrabbed(const bool grabbed) {
		glfwSetInputMode(Window::getHandle(), GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		
		Mouse::lastPosition = Mouse::getPosition();
		Mouse::velocity = glm::vec2();
	}
	bool Mouse::isGrabbed() {
		return glfwGetInputMode(Window::getHandle(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	}

	// Mesh part

	MeshBufferInfo::MeshBufferInfo() : data(), dimensions() {}
	MeshBufferInfo::MeshBufferInfo(const std::vector<float> &data, const int dimensions) {
		this->dimensions = dimensions;
		this->data = std::vector<float>(data);
	}

	GLuint Mesh::createVertexBuffer(const MeshBufferInfo &vertices, const GLuint index) {
		GLuint vboId;
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glBufferData(GL_ARRAY_BUFFER, vertices.data.size() * sizeof(float), &vertices.data[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, vertices.dimensions, GL_FLOAT, false, 0, NULL);

		return vboId;
	}

	Mesh::Mesh() {
		this->vaoId = 0;
		this->vboIds = std::vector<GLuint>();
		this->vertexCount = 0;
		this->renderMode = GL_TRIANGLES;
	}
	Mesh::Mesh(const MeshBufferInfo &vertices, const std::vector<MeshBufferInfo> &additional, const GLint renderMode) {
		this->vaoId = 0;
		this->vboIds = std::vector<GLuint>(additional.size() + 1);
		
		this->vertexCount = (GLsizei)vertices.data.size() / (GLsizei)vertices.dimensions;
		this->renderMode = renderMode;
		
		glGenVertexArrays(1, &this->vaoId);
		glBindVertexArray(this->vaoId);

		if (vertices.data.size() > 0) {
			this->vboIds[0] = Mesh::createVertexBuffer(vertices, 0);
		}

		for (size_t i = 1; i < this->vboIds.size(); ++i) {
			if (additional[i - 1].data.size() > 0) {
				this->vboIds[i] = Mesh::createVertexBuffer(additional[i - 1], (GLuint)i);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	Mesh::~Mesh() {
		this->clear();
	}

	void Mesh::load() const {
		glBindVertexArray(this->vaoId);
	}
	void Mesh::render() const {
		glDrawArrays(this->renderMode, 0, this->vertexCount);
	}
	void Mesh::clear() {
		this->unload();

		glDeleteVertexArrays(1, &this->vaoId);
		for (GLuint i : this->vboIds) glDeleteBuffers(1, &i);

		vboIds.clear();
	}

	void Mesh::unload() {
		glBindVertexArray(0);
	}

	// Shader part

	GLuint Shader::loadFromSource(const std::string &code, const GLenum type) {
		const char *c_strCode = code.c_str();

		GLuint id = glCreateShader(type);
		glShaderSource(id, 1, &c_strCode, NULL);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		std::string name = "";
		if (type == GL_VERTEX_SHADER) {
			name = "[VERTEX]";
		}
		else if (type == GL_FRAGMENT_SHADER) {
			name = "[FRAGMENT";
		}
		else if (type == GL_GEOMETRY_SHADER) {
			name = "[GEOMETRY]";
		}

		if (!success) {
			const size_t errorLength = 500;

			char error[errorLength];
			glGetShaderInfoLog(id, errorLength, NULL, error);

			std::cerr << name << " Could not compile! Error:\n" << error << '\n';
		}

		return id;
	}
	Shader Shader::loadFromFile(const std::string& path, const GLenum type) {
		std::ifstream stream{ path };
		std::stringstream sstream;

		if (stream.is_open()) {
			sstream << stream.rdbuf();
			stream.close();
		}

		return Shader(sstream.str(), type);
	}
	
	Shader::Shader(const std::string& code, const GLenum type) {
		this->id = Shader::loadFromSource(code, type);
	}

	void Shader::clear() {
		glDeleteShader(this->id);
	}
	GLuint Shader::getId() const {
		return this->id;
	}

	ShaderProgram::ShaderProgram() {
		this->id = glCreateProgram();
	}
	ShaderProgram::~ShaderProgram() {
		this->unload();
		this->clear();
	}

	void ShaderProgram::logProgramError(const GLuint id) {
		const size_t errorLength = 500;

		char error[errorLength];
		glGetProgramInfoLog(id, errorLength, NULL, error);

		std::cerr << "Could not compile program. Error:\n" << error;
	}

	void ShaderProgram::bind(const Shader shader) {
		glAttachShader(this->id, shader.getId());
		this->shaders.emplace_back(shader);
	}
	void ShaderProgram::compile() {
		glLinkProgram(this->id);
		
		GLint success;
		glGetProgramiv(this->id, GL_LINK_STATUS, &success);

		if (!success) {
			ShaderProgram::logProgramError(this->id);
		}

		glValidateProgram(this->id);
		glGetProgramiv(this->id, GL_VALIDATE_STATUS, &success);

		if (!success) {
			ShaderProgram::logProgramError(this->id);
		}
	}

	void ShaderProgram::load() const {
		glUseProgram(this->id);
	}
	void ShaderProgram::clear() {
		for (Shader& shader : this->shaders) {
			glDetachShader(this->id, shader.getId());
			shader.clear();
		}

		glDeleteProgram(this->id);
		this->shaders.clear();
	}

	void ShaderProgram::unload() {
		glUseProgram(0);
	}

	void ShaderProgram::setBoolean(const char* name, const bool value) const {
		glUniform1i(glGetUniformLocation(this->id, name), value);
	}
	void ShaderProgram::setInteger(const char* name, const int value) const {
		glUniform1i(glGetUniformLocation(this->id, name), value);
	}
	void ShaderProgram::setFloat(const char* name, const float value) const {
		glUniform1f(glGetUniformLocation(this->id, name), value);
	}
	void ShaderProgram::setVector2(const char* name, const glm::vec2 value) const {
		glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
	}
	void ShaderProgram::setVector3(const char* name, const glm::vec3 value) const {
		glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
	}
	void ShaderProgram::setVector4(const char* name, const glm::vec4 value) const {
		glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
	}
	void ShaderProgram::setMatrix2(const char* name, const glm::mat2 value) const {
		glUniformMatrix2fv(glGetUniformLocation(this->id, name), 1, false, &value[0][0]);
	}
	void ShaderProgram::setMatrix3(const char* name, const glm::mat3 value) const {
		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, false, &value[0][0]);
	}
	void ShaderProgram::setMatrix4(const char* name, const glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, &value[0][0]);
	}

	// Texture part

	GLuint Texture::loadFromFile(const std::string& path, GLint filter) {
		int width, height, channels;
		uint8_t* image = stbi_load(path.c_str(), &width, &height, &channels, 4);


		switch (channels) {
		case 3:
			channels = GL_RGB;
			break;
		case 4:
			channels = GL_RGBA;
			break;
		default:
			std::cerr << '\"' << path << '\"' << " Channels format isn't supported.\n";
			return 0;
		}

		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels, GL_UNSIGNED_BYTE, image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId;
	}

	void Texture::bind(const GLuint texture, const uint8_t bank) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0 + bank);
	}
	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture::clear(GLuint texture) {
		Texture::unbind();
		glDeleteTextures(1, &texture);
	}

	// Timer part

	Timer::Timer() {
		this->scale = 1.0f;
		
		this->update();
		this->delta = 0.0f;
	}
	Timer::Timer(const float scale) {
		this->scale = scale;
		
		this->update();
		this->delta = 0.0f;
	}

	void Timer::update() {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		int64_t currentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
		
		this->delta = (float)((double)(currentTime - this->lastTime) / 1000000000.0);
		this->lastTime = currentTime;
	}

	float Timer::getDelta() const {
		return this->delta * this->scale;
	}
	float Timer::getRealDelta() const {
		return this->delta;
	}
}
