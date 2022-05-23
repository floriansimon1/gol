#version 120

uniform ivec2     size;
uniform sampler2D state;

int wasAlive(vec2 steps, vec2 position) {
    if (clamp(position.x, 0.0, 1.0) != position.x || clamp(position.y, 0.0, 1.0) != position.y) {
        return 0;
    }

    vec4 sampled = texture2D(state, position);

    return sampled.x + sampled.y == 0.0 ? 1 : 0;
}

int countAliveNeighbors(vec2 steps, vec2 myself) {
    return (
          wasAlive(steps, myself + vec2(-1.0, -1.0) * steps)
        + wasAlive(steps, myself + vec2(-1.0, +0.0) * steps)
        + wasAlive(steps, myself + vec2(-1.0, +1.0) * steps)

        + wasAlive(steps, myself + vec2(+0.0, -1.0) * steps)
        + wasAlive(steps, myself + vec2(+0.0, +1.0) * steps)
        
        + wasAlive(steps, myself + vec2(+1.0, -1.0) * steps)
        + wasAlive(steps, myself + vec2(+1.0, +0.0) * steps)
        + wasAlive(steps, myself + vec2(+1.0, +1.0) * steps)
    );
}

bool isAlive(vec2 steps, vec2 myself) {
    int aliveNeighbors = countAliveNeighbors(steps, myself);

    return aliveNeighbors == 3 || (wasAlive(steps, myself) == 1 && aliveNeighbors == 2);
}

void main() {
    bool alive = isAlive(vec2(1.0 / size.x, 1.0 / size.y), gl_TexCoord[0].xy);

    float color = alive ? 0.0 : 1.0;

    gl_FragColor = vec4(color, color, color, 1.0);
}
