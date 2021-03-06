#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"

Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, 0.f, 0.f, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
    size = quadSize;
	position = glm::vec2(0.f);
  halfTexel = glm::vec2(sizeInSpritesheet.x / spritesheet->width(), sizeInSpritesheet.y  / spritesheet->height()); 
}

void Sprite::update(int deltaTime)
{
	if(currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe]; //+ halftexel Correccio de halftexel
	}
}

void Sprite::updateHTC(int deltaTime, int dir) //With Half Texel Correction
{
  if(currentAnimation >= 0)
  {
    timeAnimation += deltaTime;
    while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
    {
      timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
      currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
    }
    texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe] + halfTexel; // Correccio de halftexel
    if (dir == 0) texCoordDispl.x -= 2* halfTexel.x; // Correccio de halftexel
  }
}

void Sprite::render(int dir) const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
  if(dir!= 0){
    modelview = glm::scale(modelview, glm::vec3(-1.0f, 1.0f, 1.0f));
    if(size.x==24) modelview = glm::translate(modelview, glm::vec3(-32., 0.0f, 0.0f));
    else modelview = glm::translate(modelview, glm::vec3(-size.x, 0.0f, 0.0f));
  }
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::render(int dir, float rotation) const
{
  glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
  if(dir!= 0){
    modelview = glm::scale(modelview, glm::vec3(-1.0f, 1.0f, 1.0f));
    modelview = glm::translate(modelview, glm::vec3(-32.f, 0.0f, 0.0f));
  }
  modelview = glm::translate(modelview, glm::vec3(rotation*0.25, 0.0f, 0.0f));
  modelview = glm::rotate(modelview, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
  shaderProgram->setUniformMatrix4f("modelview", modelview);
  shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
  glEnable(GL_TEXTURE_2D);
  texture->use();
  glBindVertexArray(vao);
  glEnableVertexAttribArray(posLocation);
  glEnableVertexAttribArray(texCoordLocation);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if(animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0]; // - halfTexel;
	}
}

void Sprite::changeAnimationHTC(int animId, int dir) //With Half Texel Correction
{
  if(animId < int(animations.size()))
  {
    currentAnimation = animId;
    currentKeyframe = 0;
    timeAnimation = 0.f;
    texCoordDispl = animations[animId].keyframeDispl[0]  + halfTexel;
    if (dir == 0) texCoordDispl.x -= 2* halfTexel.x; // Correccio de halftexel
  }
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}
