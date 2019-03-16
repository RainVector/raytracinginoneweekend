struct ray{
    vec3 origin;
    vec3 direction;
};
   
// Estimate if ray r hit sphere
float hit_sphere(vec3 center, float radius, ray r)
{
    vec3 oc = r.origin - center;
    float a = dot(r.direction,r.direction);
    float b = float(2.0) * dot(oc, r.direction);
    float c = dot(oc,oc)- radius*radius;
    float discriminant = b * b - 4.0 * a * c;
    if (discriminant<0.0)
        return -1.0;
    else
        return (-b - sqrt(discriminant))/(2.0*a);
}
    
vec3 color(ray r)
{
    vec3 sphereCenter = vec3(0.0,0.0,-1.0);
    float t = hit_sphere(sphereCenter,0.5,r);
    if(t > 0.0)
    {
        // point at parameter
        vec3 p = r.origin + t * r.direction;
    	vec3 N = normalize(p - sphereCenter);
        return 0.5 * vec3(N.x + 1.0, N.y + 1.0, N.z + 1.0);
    }
    
    vec3 unit_direction = normalize(r.direction);
    t = 0.5 * (unit_direction.y + 1.0);
    vec3 result = (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5,0.7,1.0);
    return result;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // lower 
    float ratio = iResolution.y/iResolution.x;
    vec3 lower_left_corner = vec3(-2.0,-1.0,-1.0);
    vec3 horizontal = vec3(4.0,0.0,0.0);
    vec3 vertical   = vec3(0.0,4.0*ratio,0.0);
    vec3 origin     = vec3(0.0,0.0,0.0);
    

    float u = fragCoord.x/iResolution.x;
    float v = fragCoord.y/iResolution.y;
    ray a;
    a.origin = origin;
    a.direction = lower_left_corner + u * horizontal + v * vertical;
    vec3 col = color(a);
     
    // Output to screen
    fragColor = vec4(col,1.0);
}