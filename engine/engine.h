#pragma once

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <chrono>

#include <vector>
#include <sstream>

#include "../include/glm/glm.hpp"
#include "../include/stb_image.h"

#define GLFW_INCLUDE_NONE
#include "../include/GLFW/glfw3.h"

#include "../include/glad/glad.h"

#include "logger/logger.h"

namespace Engine {
	// Window part

	struct WindowCreateInfo {
		std::string title;
		int width, height;

		bool resizable;
		bool vSyncEnabled;

		WindowCreateInfo();
	};
	class Window {
	private:
		static GLFWwindow *handle;
		static int x, y, width, height;
		
		static bool running, created;

		static void onResize(GLFWwindow* handle, int width, int height);
		static void onMove(GLFWwindow* handle, int x, int y);
	public:
		static void create(const WindowCreateInfo& createInfo);
		static void pollEvents();
		static void swapBuffers();

		static void close();

		static void setSize(const int width, const int height);

		static void setWidth(const int width);
		static void setHeight(const int height);

		static void setPosition(const int x, const int y);
		static void setX(const int x);
		static void setY(const int y);

		static void lockAspect(const int numer, const int denom);
		static void unlockAspect();

		static void setTitle(const std::string& title);

		static GLFWwindow* getHandle();

		static bool isRunning();

		static int getWidth();
		static int getHeight();

		static glm::ivec2 getSize();

		static int getX();
		static int getY();

		static glm::ivec2 getPosition();
	};

	// Keyboard part

	class Keyboard {
	private:
		static std::unordered_map<uint16_t, uint64_t> keys;
		static uint64_t currentFrame;
	public:
		static void update();
		
		static void onKeyPress(const uint16_t key);
		static void onKeyRelease(const uint16_t key);

		static bool isKeyPressed(const uint16_t key);
		static bool isKeyJustPressed(const uint16_t key);
	};

	// Mouse part

	class Mouse {
	private:
		static uint64_t buttons[GLFW_MOUSE_BUTTON_LAST + 1];
		static uint64_t currentFrame;

		static glm::vec2 lastPosition, velocity;
	public:
		static void update();

		static void onButtonPress(const uint8_t button);
		static void onButtonRelease(const uint8_t button);

		static bool isButtonPressed(const uint8_t button);
		static bool isButtonJustPressed(const uint8_t button);

		static glm::vec2 getPosition();
		static glm::vec2 getVelocity();

		static void setGrabbed(const bool grabbed);
		static bool isGrabbed();
	};

	// Mesh part

	struct MeshBufferInfo {
		std::vector<float> data;
		int dimensions;

		MeshBufferInfo();
		MeshBufferInfo(const std::vector<float> &data, const int dimensions);
	};
	class Mesh {
	private:
		GLuint vaoId;
		std::vector<GLuint> vboIds;

		GLsizei vertexCount;
		GLint renderMode;
		
		static GLuint createVertexBuffer(const MeshBufferInfo &vertices, const GLuint index);
	public:
		Mesh();
		Mesh(const MeshBufferInfo &vertices, const std::vector<MeshBufferInfo> &additional, const GLint renderMode);
		~Mesh();

		void load() const;
		void render() const;
		void clear();

		static void unload();
	};

	// Shader part

	class Shader {
	private:
		GLuint id;
		static GLuint loadFromSource(const std::string& code, const GLenum type);
	public:
		Shader(const std::string& code, const GLenum type);
		static Shader loadFromFile(const std::string &path, const GLenum type);

		void clear();
		GLuint getId() const;
	};
	class ShaderProgram {
	private:
		GLuint id;
		std::vector<Shader> shaders;

		static void logProgramError(const GLuint id);
	public:
		ShaderProgram();
		~ShaderProgram();

		void bind(const Shader shader);
		void compile();

		void load() const;
		void clear();

		void setBoolean(const char* name, const bool value) const;
		void setInteger(const char* name, const int value) const;
		void setFloat(const char* name, const float value) const;
		void setVector2(const char* name, const glm::vec2 value) const;
		void setVector3(const char* name, const glm::vec3 value) const;
		void setVector4(const char* name, const glm::vec4 value) const;
		void setMatrix2(const char* name, const glm::mat2 value) const;
		void setMatrix3(const char* name, const glm::mat3 value) const;
		void setMatrix4(const char* name, const glm::mat4 value) const;

		static void unload();
	};

	// Texture part

	class Texture {
	public:
		static GLuint loadFromFile(const std::string& path, const GLint filter);

		static void bind(const GLuint texture, const uint8_t bank);
		static void unbind();
		static void clear(GLuint texture);
	};
	
	// Timer part

	class Timer {
	private:
		int64_t lastTime;
		float delta;
	public:
		float scale;
		
		Timer();
		Timer(const float scale);
		
		void update();

		float getDelta() const;
		float getRealDelta() const;
	};
}