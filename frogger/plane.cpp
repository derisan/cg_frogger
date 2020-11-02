#include "plane.h"

#include <GL/glew.h>

#include "vertexarray.h"

Plane::Plane(Game* game)
	: Object{ game },
	mVertexArray{ nullptr }
{
	Load();
}

Plane::~Plane()
{
	delete mVertexArray;
}

void Plane::UpdateObject()
{
	Object::UpdateObject();
}

void Plane::Draw(Shader* shader)
{
	Object::Draw(shader);

	mVertexArray->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Plane::Load()
{
	const float vertices[] = {
		 -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	mVertexArray = new VertexArray{ vertices, static_cast<unsigned int>(sizeof(vertices) / sizeof(float)) };
}