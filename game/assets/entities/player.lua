canJump = true;

function init(entity)
end

function update(entity)
    velocity = entity:getVelocityComponent();
    skelet = entity:getSkeletComponent();
    local run = false;
    local rotate = false;

    if Input.isKeyPressed(Input.Key.W) then
        velocity.speed = 0.05;
        run = true;
    elseif Input.isKeyPressed(Input.Key.S) then
        velocity.speed = -0.05;
        run = true;
    else
        velocity.speed = 0;
        run = false;
    end

    if Input.isKeyPressed(Input.Key.D) then
        velocity.rotation.y = 0.03;
        rotate = true;
    elseif Input.isKeyPressed(Input.Key.A) then
        velocity.rotation.y = -0.03;
        rotate = true;
    else
        velocity.rotation.y = 0;
        rotate = false;
    end

    if (Input.isKeyPressed(Input.Key.Space) and canJump) then
        velocity.velocity.y = 0.2;
        canJump = false;
    end

    if (run or rotate or not canJump) then
        skelet.state:play("Armature|Run");
    else
        skelet.state:play("Armature|Idle");
    end
end

function collide(entity, other)
    local otherCollision = other:getStaticCollisionComponent();
    if (otherCollision.tag == "platform") then
        local location = entity:getLocationComponent();
        location.parent = other:getId();
        canJump = true;
    end
end