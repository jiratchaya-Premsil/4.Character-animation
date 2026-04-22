#include "gameLogic/customer.h"
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>
#include <learnopengl/shader_m.h>

#include <cstdlib> // rand
#include <ctime> // time
#include <iostream>

Customer::Customer() {
	speed = 2.0f;
	state = CustomerState::Walking;
	stateTimer = 0.0f;
	greeted = false;
	readyToLeave = false;

	// Seed random number generator if not already done
	static bool seeded = false;
	if (!seeded) {
		srand(static_cast<unsigned int>(time(nullptr)));
		seeded = true;
	}
}

void Customer::Init(const glm::vec3& startPos, const glm::vec3& targetPos,
	AnimationModel* sharedModel,
	Animation* walkAnim,
	Animation* greetAnim,
	Animation* idleAnim)
{
	position = startPos;
	targetA = startPos;
	targetB = targetPos;

	baseModel = sharedModel;

	walkAnimation = walkAnim;
	greetAnimation = greetAnim;
	idleAnimation = idleAnim;

	animator = std::make_unique<Animator>(walkAnimation);

	//clothOptions = {
	//	std::make_shared<AnimationModel>("resources/customer/cloth1.obj"),
	//	std::make_shared<AnimationModel>("resources/customer/cloth2.obj")
	//};

	//pantsOptions = {
	//	std::make_shared<AnimationModel>("resources/customer/pants1.obj"),
	//	std::make_shared<AnimationModel>("resources/customer/pants2.obj")
	//};

	//SetRandomOutfit();

	ChangeState(CustomerState::Walking);
}

void Customer::SetRandomOutfit()

{
	if (clothOptions.empty() )return;

	int clothIndex = rand() % clothOptions.size();
	int pantsIndex = rand() % pantsOptions.size();

	clothModel = clothOptions[clothIndex];
	pantsModel = pantsOptions[pantsIndex];
}

void Customer::ChangeOutfit()
{
	SetRandomOutfit();
}

void Customer::ChangeState(CustomerState newState)
{
	state = newState;
	stateTimer = 0.0f;

	switch (state)
	{
	case CustomerState::Walking:
	case CustomerState::Leaving:
		animator->PlayAnimation(walkAnimation);
		readyToLeave = false;
		break;

	case CustomerState::Greeting:
		animator->PlayAnimation(greetAnimation);
		greeted = false;
		readyToLeave = false;
		break;

	case CustomerState::Idle:
		animator->PlayAnimation(idleAnimation);
		break;
	}
}

void Customer::Update(float deltaTime)
{
	stateTimer += deltaTime;

	// 🔥 update animation every frame
	animator->UpdateAnimation(deltaTime);

	switch (state)
	{
	case CustomerState::Walking:
		MoveTo(targetB, deltaTime);

		if (glm::distance(position, targetB) < 0.1f)
		{
			ChangeState(CustomerState::Idle);
		}
		break;

	case CustomerState::Greeting:
		if (!greeted)
		{
			greeted = true;
		}

		if (stateTimer > 5.0f)
		{
			ChangeState(CustomerState::Idle);
		}
		break;

	case CustomerState::Idle:
		if (readyToLeave && stateTimer > 3.0f)
		{
			ChangeState(CustomerState::Leaving);
		}
		break;

	case CustomerState::Leaving:
		MoveTo(targetA, deltaTime);

		if (glm::distance(position, targetA) < 0.1f)
		{
			Respawn();
			ChangeOutfit();

		}
		break;
	}
}

void Customer::Interact()
{
	if (state == CustomerState::Idle)
	{
		if (!greeted) {
			ChangeState(CustomerState::Greeting);
		}
		else if (!readyToLeave) {
			readyToLeave = true;
			stateTimer = 0.0f;
		}
	}
}

void Customer::Respawn()
{
    // Randomize new start position (x between 2.0 and 4.0)
	float newStartX = 10.0f + (static_cast<float>(rand()) / RAND_MAX * 2.0f);
    position = glm::vec3(position.x, position.y, newStartX);
    targetA = position;
    // targetB remains the same (the counter position)

    greeted = false;
    readyToLeave = false;
    speach.clear();
    ChangeState(CustomerState::Walking);
}

void Customer::MoveTo(const glm::vec3& target, float deltaTime)
{
	glm::vec3 dir = target - position;
	if (glm::length(dir) > 0.001f)
	{
		dir = glm::normalize(dir);
		position += dir * speed * deltaTime;
	}
}

void Customer::Draw(Shader& shader)
{
    Draw(shader, 1.0f);
}

void Customer::Draw(Shader& shader, float scale)
{
	auto transforms = animator->GetFinalBoneMatrices();

	for (int i = 0; i < transforms.size(); ++i)
	{
		shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
	}

	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(scale));
    shader.setMat4("model", model);

	baseModel->Draw(shader);
	//if (clothModel) clothModel->Draw(shader);
	//if (pantsModel) pantsModel->Draw(shader);
}