function main(app)
    app:loadModel3D_v2("frog", "./assets/models/man/Man8.fbx");

    app:loadModel3D_v2("box", "./assets/models/box/box.fbx");

    -- app:loadModel3D("cottage", "./assets/models/cottage/cottage.obj",
    --                 "./assets/models/cottage/cottage_diffuse.png",
    --                 "./assets/models/cottage/cottage_specular.png",
    --                 "./assets/models/cottage/cottage_normal.png");

    app:setSkybox("./assets/models/skybox-2/right.png",
                  "./assets/models/skybox-2/left.png",
                  "./assets/models/skybox-2/top-2.png",
                  "./assets/models/skybox-2/bottom.png",
                  "./assets/models/skybox-2/back.png",
                  "./assets/models/skybox-2/front.png");

    local player = app:createEntity("player");
    player:addTagComponent(Core.TagComponent("player"));
    local location = Core.LocationComponent(0.0, 2.0, 0.0);
    player:addLocationComponent(location);
    player:addVelocityComponent(Core.VelocityComponent());
    local render = Core.Render3DComponent("frog", 0.25);
    render.rotation.y = 3.14;
    player:addRender3DComponent(render);
    player:addCollisionComponent(Core.CollisionComponent(0.5, 0.65, 0.5));
    player:addPhysicsComponent(Core.PhysicsComponent(0.01));
    -- player:addCameraComponent(Core.CameraComponent(-4.0, 1.0, 0.0));
    player:addSkeletComponent(Core.SkeletComponent("Armature|Idle"));

    app:loadTerrain("terrain", "./assets/heightmap.png", 50, 50, 5.0);
    local terrain = app:createEntity("terrain");
    terrain:addTagComponent(Core.TagComponent("terrain"));
    terrain:addLocationComponent(Core.LocationComponent(-50.0, 0.0, -50.0));
    terrain:addStaticRender3DComponent(Core.StaticRender3DComponent("terrain", 1));
    terrain:addTerrainCollisionComponent(Core.TerrainCollisionComponent(50, 50));
    terrain:addTagComponent(Core.TagComponent("platform"));

    local frog = app:createEntity("frog");
    frog:addTagComponent(Core.TagComponent("frog"));
    frog:addLocationComponent(Core.LocationComponent(10.0, 0.0, 10.0));
    frog:addVelocityComponent(Core.VelocityComponent());
    local render = Core.Render3DComponent("frog", 1.0);
    render.rotation.y = 1.57;
    frog:addRender3DComponent(render);

    local light = app:createEntity("light");
    light:addTagComponent(Core.TagComponent("light"));
    local lightProps = Core.DirectedLight();
    lightProps.ambient = Core.vec3(0.1, 0.1, 0.1);
    lightProps.diffuse = Core.vec3(0.3, 0.3, 0.3);
    lightProps.specular = Core.vec3(1.0, 1.0, 1.0);
    lightProps.farPlane = 30;
    light:addDirectedLightComponent(Core.DirectedLightComponent(lightProps));

    local fire = app:createEntity("fire");
    fire:addTagComponent(Core.TagComponent("fire"));
    fire:addLocationComponent(Core.LocationComponent(0.0, 2.0, 0.0));
    fire:addRender3DComponent(Core.Render3DComponent("box", 0.1));
    local emitter = Core.ParticlesConfiguration();
    emitter.count = 1000;
    emitter.rate = 0.002;
    emitter.sizeFrom = 100.0;
    emitter.sizeTo = 0.0;
    emitter.lifeTime = 1.0;
    emitter.spawnRadius = 0.0;
    emitter.velocityMin = 1.0;
    emitter.velocityMax = 2.0;
    emitter.velocityPolarAngle = 3.14 / 10;
    emitter.velocityAzimuthalAngle = 6.28;
    emitter.colorFrom = Core.vec3(0.0, 0.0, 0.0);
    emitter.colorTo = Core.vec3(1.0, 1.0, 1.0);
    emitter.gravity = Core.vec3(0.0, -0.5, 0.0);
    fire:addParticlesComponent(Core.ParticlesComponent(emitter));
    fire:addStaticCollisionComponent(Core.StaticCollisionComponent(0.2, 0.2, 0.2));

    for j = 0, 1 do

        for i = 0, 10 do
            local box = app:createEntity("box0" .. i);
            box:addLocationComponent(Core.LocationComponent(0.0, j * -2.0, i * 2));
            box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
            box:addTagComponent(Core.TagComponent("platform"));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

        for i = 0, 10 do
            local box = app:createEntity("box1" .. i);
            box:addLocationComponent(Core.LocationComponent(i * 2, j * -2.0, 20.0));
            box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
            box:addTagComponent(Core.TagComponent("platform"));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

        for i = 0, 10 do
            local box = app:createEntity("box2" .. i);
            box:addLocationComponent(Core.LocationComponent(20, j * -2.01, 20.0 - i * 2));
            box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
            box:addTagComponent(Core.TagComponent("platform"));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

        for i = 0, 10 do
            local box = app:createEntity("box3" .. i);
            box:addLocationComponent(Core.LocationComponent(20.0 - i * 2, j * -2.0, 0.0));
            box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
            box:addTagComponent(Core.TagComponent("platform"));
            box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
        end

    end

    for i = 1, 3 do
        local box = app:createEntity("box4" .. i);
        box:addLocationComponent(Core.LocationComponent(10, 2 * i, 20.0));
        box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
        box:addTagComponent(Core.TagComponent("platform"));
        box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
    end

    for i = 1, 5 do
        local box = app:createEntity("box5" .. i);
        box:addLocationComponent(Core.LocationComponent(14, 2 * i, 20.0));
        box:addRender3DComponent(Core.Render3DComponent("box", 1.0));
        box:addTagComponent(Core.TagComponent("platform"));
        box:addStaticCollisionComponent(Core.StaticCollisionComponent(2.0, 2.0, 2.0));
    end
end
