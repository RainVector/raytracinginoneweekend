struct ray{
    vec3 origin;
    vec3 direction;
};
   
vec3 color(ray r)
{
    vec3 unit_direction = normalize(r.direction);
    float t = 0.5 * (unit_direction.y + 1.0);
    vec3 result = (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5,0.7,1.0);
    return result;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // lower 
    vec3 lower_left_corner = vec3(-2.0,-1.0,-1.0);
    vec3 horizontal = vec3(4.0,0.0,0.0);
    vec3 vertical   = vec3(0.0,2.0,0.0);
    vec3 origin     = vec3(0.0,0.0,0.0);
    

    float u = fragCoord.x/iResolution.x;
    float v = fragCoord.y/iResolution.y;
    ray a;
    a.origin = origin;
    a.direction = lower_left_corner + u * horizontal + v * vertical;
    vec3 col = color(a);
     
    // Output to screen
    fragColor = vec4(col,1.0);

    float d = 1.0;
    if (d < 0.0)
        fragColor = vec4(1.0,0.0,0.0,1.0);
}