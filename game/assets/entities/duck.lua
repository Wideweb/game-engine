canJump = true;

function init(entity)
end

function update(entity)
    velocity = entity:getVelocityComponent();
    velocity.speed = 1.0;
    velocity.rotation.y = 0.001;
end

function collide(entity, other)
    local tag = other:getTagComponent().tag;
    if (tag == "platform") then
        local location = entity:getLocationComponent();
        location.parent = other:getId();
        canJump = true;
    end
end