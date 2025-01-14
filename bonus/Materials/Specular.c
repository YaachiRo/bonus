/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 06:21:34 by olahrizi          #+#    #+#             */
/*   Updated: 2024/01/03 18:37:03 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header.h"

static int cast_ray(t_object *objects, t_ray cast_ray, int count)
{
	t_info test;
	int i;
	int validint;
	i = 0;
	while (i < count)
	{
		test.e = &objects[i];
		validint = objects[i].intersect(&cast_ray, &test);
		if (validint)
			return (1);
		i++;
	}
	return (0);
}

float get_intensity(t_vec3 light_ray_dir, t_vec3 normal, t_vec3 cam_dir, t_object *e)
{
	t_vec3 v;
	t_vec3 ref_v;
	float prod;

	v = normalized(cam_dir);
	ref_v = reflect(normalized(light_ray_dir), normal);
	prod = dot_product(ref_v, v);
	if (prod > 0.0)
		return (e->reflectivity * powf(prod, e->shininess));
	return (0);
}

t_vec3 specular_highlight(t_vars *vars, t_info *info, t_ray *camera_ray)
{
	t_vec3 spc_color;
	float red;	
	float green;	
	float blue;
	int j;
	t_vec3 lighdir;
	t_vec3 start;
	t_vec3 point2;
	t_ray lightray;
	float intensity;
	int validint;

	red = .0f;	
	green = .0f;	
	blue = .0f;
	j = 0;
	intensity = 0.0f;
	while (j < vars->l_count)
	{
		lighdir = normalized(vec_sub(vars->lights[j].position, info->hitpoint));
		start = vec_add(info->hitpoint, scale_vector(lighdir, 0.0001f));
		point2 = vec_add(start, lighdir);
		lightray = new_ray(start, point2);
		validint = cast_ray(vars->objects, lightray, vars->obj_count);
		if (!validint)
			intensity = get_intensity(lightray.dir, info->localnormal, camera_ray->dir, info->e);
		red += vars->lights[j].color.x * intensity * vars->lights[j].intensity;
		green += vars->lights[j].color.y * intensity * vars->lights[j].intensity;
		blue += vars->lights[j].color.z * intensity * vars->lights[j].intensity;
		j++;
	};
	spc_color = new_vector(red, green, blue);
	return (spc_color);
}
