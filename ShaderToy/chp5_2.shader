struct ray    {vec3 origin; vec3 direction;};
  
struct sphere{ vec3 center; float radius;}; 
        
struct hit_record {bool isHit; float t; vec3 p; vec3 n; };
    
    
// Estimate if ray r hit sphere
hit_record hit_sphere(ray r, sphere s, float t_min, float t_max)
{
    hit_record ht; ht.isHit = false;
    vec3 center  = s.center;
    float radius = s.radius;
    
    vec3 oc = r.origin - center;
    float a = dot(r.direction,r.direction);
    float b = dot(oc, r.direction);
    float c = dot(oc,oc)- radius*radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0.0)
    {
    	float temp = (-b - sqrt(discriminant))/a;
        if(temp < t_max && temp > t_min)
        {
        	ht.t = temp;
            ht.p = r.origin + temp * r.direction;
            ht.n = (ht.p - center) / radius;
            ht.isHit = true;
            return ht;
        }

    	temp = (-b + sqrt(discriminant))/a;
        if(temp < t_max && temp > t_min)
        {
            ht.t = temp;
            ht.p = r.origin + temp * r.direction;
            ht.n = (ht.p - center)/radius;
            ht.isHit = true;
            return ht;
        }
    }
    return ht;
}
    
vec3 color(ray r, sphere hitableList[2], int num)
{
    for(int i = 0; i <num; i++)
    {
        hit_record ht = hit_sphere(r,hitableList[i],0.0,10000.0);
    	if(ht.isHit)
        {
        	return 0.5 * vec3(ht.n.x+1.0,ht.n.y+1.0,ht.n.z+1.0);
        }
        else{
        	vec3 unit_direction = normalize(r.direction);
    		float t = 0.5 * (unit_direction.y + 1.0);
    		vec3 result = (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5,0.7,1.0);
    		return result;
        }
    }   
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{ 
    float ratio = iResolution.y/iResolution.x;
    vec3 lower_left_corner = vec3(-2.0,-1.0,-1.0);
    vec3 horizontal = vec3(4.0,0.0,0.0);
    vec3 vertical   = vec3(0.0,4.0*ratio,0.0);
    vec3 origin     = vec3(0.0,0.0,0.0);
    
    float u = fragCoord.x/iResolution.x;
    float v = fragCoord.y/iResolution.y;
    ray r;
    r.origin = origin;
    r.direction = lower_left_corner + u * horizontal + v * vertical;
    
    sphere s1,s2;
    s1.center = vec3(0.0,   0.0,-1.0), s1.radius = 0.5;
    s2.center = vec3(0.0,-100.5,-1.0), s2.radius = 100.0;
    sphere[] hitableList = sphere[](s1,s2);
    
    vec3 col = color(r, hitableList,2);
     
    // Output to screen
    fragColor = vec4(col,1.0);
}