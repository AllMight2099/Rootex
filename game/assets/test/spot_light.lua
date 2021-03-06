Entity = {
    Components = {
        TransformComponent = {
            m_Position = {x = 0, y = 0, z = 0},
            m_Rotation = {yaw = 0, pitch = 3.14, roll = 0},
            m_Scale = {x = .1, y = .1, z = .1}
        },
        SpotLightComponent = {
            attLin = 0.045,
            attQuad = 0.0075,
            attConst = 1.0,
            range = 100.0,
            diffuseIntensity = 1.0,
            diffuseColor = {r = 1.0, g = 1.0, b = 1.0, a = 1.0},
            ambientColor = {r = 0.05, g = 0.05, b = 0.05, a = 1.0},
            spot = 4,
            angleRange = 30
        },
        DiffuseVisualComponent = {
            resFile = "game/assets/test/sphere.obj",
            texturePath = "game/assets/test/yellow.png"
        }
    }
}
