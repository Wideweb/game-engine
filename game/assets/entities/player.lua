function init(entity)
end

function update(entity)
    velocity = entity:getVelocityComponent();

    if Input.isKeyPressed(Input.Key.W) then
        velocity.speed = 0.08;
    end

    if Input.isKeyPressed(Input.Key.S) then
        velocity.speed = -0.08;
    end

    if Input.isKeyPressed(Input.Key.D) then
        velocity.rotation.y = 0.03;
      elseif Input.isKeyPressed(Input.Key.A) then
        velocity.rotation.y = -0.03;
    else
        velocity.rotation.y = 0;
    end

    if Input.isKeyPressed(Input.Key.Space) then
        velocity.velocity.y = 0.2;
    end
end