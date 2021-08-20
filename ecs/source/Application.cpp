#include "gui/Application.hpp"

namespace
{
    constexpr float VIEW_HEIGHT = 600.0f;
    constexpr float FRICTION_RATE = 0.01f;
}
Application::Application()
: mWindow(sf::VideoMode(800, 600), "Game", sf::Style::Default)
, mView(mWindow.getDefaultView())
, TIME_PER_FRAME(sf::seconds(1/60.0f))
, mTextures()
{
    setup();
    mWindow.setFramerateLimit(60);
    mWindow.setKeyRepeatEnabled(false);
}

Application::~Application()
{

}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen())
    {
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate >= TIME_PER_FRAME)
        {
            process();
            update(TIME_PER_FRAME);
            timeSinceLastUpdate -= TIME_PER_FRAME;
        }
        render();
    }
}

void Application::process()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();
        if(event.type == sf::Event::Resized)
        {
            mView.setSize((float)mWindow.getSize().x/mWindow.getSize().y * VIEW_HEIGHT,
                          VIEW_HEIGHT);
        }
        mListener->handleEvent(event);
    }
    mListener->update(sf::Time::Zero);
}

void Application::update(sf::Time dt)
{
    mGravity->update(dt);
    mMover->update(dt);
    mRigidSystem->update(dt);
    mHealth->update(dt);
    mCollider->update(dt);
    mPositioner->update(dt);

    if(mHealth->isEntityAlive(mEntities[0]))
    {
        sf::Vector2f viewPos = mView.getCenter();
        sf::FloatRect playerPos = mECSE.getComponent<Transform>(mEntities[0]).rect;
        viewPos.x += (playerPos.left - viewPos.x)*std::pow(FRICTION_RATE, dt.asSeconds());
        viewPos.y = 300.0f;
        mView.setCenter(viewPos);
    }
}

void Application::render()
{
    mWindow.setView(mView);
    mWindow.clear();
    mRenderer->update(sf::Time::Zero);
    mHealth->render();
    mWindow.display();
}

void Application::setup()
{
    mTextures.load(Textures::Concrete, "asset/texture/Concrete.png");
    mTextures.load(Textures::Grass, "asset/texture/Grass.png");
    mTextures.load(Textures::Earth, "asset/texture/Earth.png");

    //register components
    mECSE.registerComponent<Transform>();
    mECSE.registerComponent<RigidBody>();
    mECSE.registerComponent<Health>();
    mECSE.registerComponent<Render>();
    mECSE.registerComponent<Movement>();
    mECSE.registerComponent<Jump>();
    mECSE.registerComponent<HealthBox>();

    //register events
    mECSE.registerEventType<Movement>();
    mECSE.registerEventType<CollisionInfo>();
    mECSE.registerEventType<TouchGround>();

    //register systems
    mRenderer = mECSE.registerSystem<RenderSystem>();
    mRigidSystem = mECSE.registerSystem<RigidBodySystem>();
    mPositioner = mECSE.registerSystem<PositionSystem>();
    mGravity = mECSE.registerSystem<GravitySystem>();
    mMover = mECSE.registerSystem<InputSystem>();
    mListener = mECSE.registerSystem<EventSystem>();
    mCollider = mECSE.registerSystem<CollisionSystem>();
    mHealth = mECSE.registerSystem<HealthSystem>();

    //set-up systems
    mRenderer->setWindow(&mWindow);
    mRenderer->setECSE(&mECSE);
    mRenderer->loadTexture(&mTextures);

    mRigidSystem->setECSE(&mECSE);

    mPositioner->setECSE(&mECSE);

    mGravity->setECSE(&mECSE);

    mMover->setECSE(&mECSE);

    mListener->setECSE(&mECSE);

    mCollider->setECSE(&mECSE);

    mHealth->setECSE(&mECSE);
    mHealth->setWindow(&mWindow);

    //subcribe events
    mECSE.addEventSubcriber<Movement>(mMover.get());
    mECSE.addEventSubcriber<CollisionInfo>(mRigidSystem.get());
    mECSE.addEventSubcriber<TouchGround>(mGravity.get());

    //set event manager to systems in need
    mMover->setEventManager(mECSE.getEventManager());
    mListener->setEventManager(mECSE.getEventManager());
    mCollider->setEventManager(mECSE.getEventManager());
    mRigidSystem->setEventManager(mECSE.getEventManager());
    mGravity->setEventManager(mECSE.getEventManager());

    //signature
    Signature signature;
    signature.set(mECSE.getComponentType<Transform>(), true);
    signature.set(mECSE.getComponentType<Render>(), true);
    mECSE.setSystemSignature<RenderSystem>(signature);

    signature.reset();
    signature.set(mECSE.getComponentType<Transform>(), true);
    signature.set(mECSE.getComponentType<RigidBody>(), true);
    mECSE.setSystemSignature<RigidBodySystem>(signature);

    signature.reset();
    signature.set(mECSE.getComponentType<Transform>(), true);
    signature.set(mECSE.getComponentType<RigidBody>(), true);
    mECSE.setSystemSignature<PositionSystem>(signature);

    signature.reset();
    signature.set(mECSE.getComponentType<Transform>(), true);
    signature.set(mECSE.getComponentType<RigidBody>(), true);
    signature.set(mECSE.getComponentType<Jump>(), true);
    mECSE.setSystemSignature<GravitySystem>(signature);

    signature.reset();
    signature.set(mECSE.getComponentType<Transform>(), true);
    signature.set(mECSE.getComponentType<RigidBody>(), true);
    signature.set(mECSE.getComponentType<Movement>(), true);
    signature.set(mECSE.getComponentType<Jump>(), true);
    mECSE.setSystemSignature<InputSystem>(signature);

    signature.reset();
    mECSE.setSystemSignature<EventSystem>(signature);

    signature.reset();
    signature.set(mECSE.getComponentType<Transform>(), true);
    signature.set(mECSE.getComponentType<RigidBody>(), true);
    mECSE.setSystemSignature<CollisionSystem>(signature);

    signature.reset();
    signature.set(mECSE.getComponentType<Transform>(), true);
    signature.set(mECSE.getComponentType<Health>(), true);
    signature.set(mECSE.getComponentType<HealthBox>(), true);
    mECSE.setSystemSignature<HealthSystem>(signature);

    //create entity
    //moving block
    {
    Entity concrete = mECSE.createEntity();
    mEntities.push_back(concrete);
    mECSE.addComponent(concrete,
        Transform
        {
            .rect = sf::FloatRect(0.0f, 0.0f, 80.0f, 160.0f),
        });

    mECSE.addComponent(concrete,
        Render
        {
            .texture = Textures::Concrete,
        });

    mECSE.addComponent(concrete,
        RigidBody
        {
            .velocity = sf::Vector2f(0.0f, 0.0f),
            .acceleration = sf::Vector2f(0.0f, 0.0f),
            .slowFriction = 0.2f,
        });

    mECSE.addComponent(concrete,
        Jump
        {
            .gravity = 500.0f,
            .jumpHeight = 100.0f,
            .maxJump = 2,
            .currentJump = 0,
        });

    mECSE.addComponent(concrete,
        Movement
        {
            .direction = sf::Vector2f(0.0f, 0.0f),
            .absAcceleration = 500.0f,
        });

    mECSE.addComponent(concrete,
        Health
        {
            .maxHP = 100,
            .currentHP = 100,
        });

    mECSE.addComponent(concrete,
        HealthBox
        {
            .hasHealth = true,
            .hurtBox = sf::FloatRect(0.0f, 0.0f, 100.0f, 100.0f),
            .hasHitbox = false,
            .hitBox = sf::FloatRect(),
            .damage = 0,
        });
    }
    //static blocks
    {
    Entity ground1 = mECSE.createEntity();
    mEntities.push_back(ground1);
    mECSE.addComponent(ground1,
        Render
        {
            .texture = Textures::Earth,
        });

    mECSE.addComponent(ground1,
        Transform
        {
            .rect = sf::FloatRect(175.0f, 350.0f, 50.0f, 50.0f),
        });

    mECSE.addComponent(ground1,
        RigidBody
        {
            .velocity = sf::Vector2f(),
            .acceleration = sf::Vector2f(),
        });

    mECSE.addComponent(ground1,
        Health
        {
            .maxHP = 1,
            .currentHP = 1,
        });

    mECSE.addComponent(ground1,
        HealthBox
        {
            .hasHealth = false,
            .hurtBox = sf::FloatRect(),
            .hasHitbox = true,
            .hitBox = sf::FloatRect(0.0f, 0.0f, 50.0f, 50.0f),
            .damage = 10,
        });

    Entity ground2 = mECSE.createEntity();
    mEntities.push_back(ground2);
    mECSE.addComponent(ground2,
        Render
        {
            .texture = Textures::Grass,
        });

    mECSE.addComponent(ground2,
        Transform
        {
            .rect = sf::FloatRect(0.0f, 400.0f, 500.0f, 100.0f),
        });

    mECSE.addComponent(ground2,
        RigidBody
        {
            .velocity = sf::Vector2f(),
            .acceleration = sf::Vector2f(),
        });

    Entity ground3 = mECSE.createEntity();
    mEntities.push_back(ground3);
    mECSE.addComponent(ground3,
        Render
        {
            .texture = Textures::Grass,
        });

    mECSE.addComponent(ground3,
        Transform
        {
            .rect = sf::FloatRect(300.0f, 300.0f, 100.0f, 100.0f),
        });

    mECSE.addComponent(ground3,
        RigidBody
        {
            .velocity = sf::Vector2f(),
            .acceleration = sf::Vector2f(),
        });
    }
}
