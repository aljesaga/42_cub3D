/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatute- <jmatute-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 18:32:27 by jmatute-          #+#    #+#             */
/*   Updated: 2022/12/01 19:07:47 by jmatute-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cub3d.h"

#define LENGHT_RAY 15

void dda_line(int xi, int yi, int xf, int yf, mlx_image_t *flor, uint32_t color)
{
	t_dcords	dcords;
	int			i;
	
	i = 0;
	dcords.dx = xf - xi;
	dcords.dy = yf - yi;
	if (fabs(dcords.dx) >= fabs(dcords.dy))
		dcords.p = fabs(dcords.dx);
	else
		dcords.p = fabs(dcords.dy);
	dcords.incx = dcords.dx / dcords.p;
	dcords.incy = dcords.dy / dcords.p;
	dcords.x = xi;
	dcords.y = yi;
	while(i < dcords.p){
		 mlx_put_pixel(flor, dcords.x, dcords.y, color);
		 dcords.x += dcords.incx;
		 dcords.y += dcords.incy;
		 i++;
	}
}

int fill_window(int l_image, t_env *env)
{
	int y;
	int x;

	y = 0;
	x = 0;
	while (y  < env->height)
	{
		while (x < env->width){
			if (env->map[y][x] == '1')
				;//mlx_image_to_window(env->mlx, env->walls, x * l_image, y * l_image);
			else if (env->map[y][x] == 'P'){
				;//mlx_image_to_window(env->mlx, env->player ,x *l_image ,y * l_image);
				env->x = x * l_image + 5;
				env->y = y * l_image + 5;
			}
			x++;
		}
		x = 0;
		y ++;
	}
	//draw_separator(&env);
	return (0);
}

void vortice_hook(void *param){
	t_env *env;
	env = param;
	
	if (env->pa > 2 *PI)
		env->pa -= (2 * PI);
	if (env->pa < 0)
		env->pa += (2 * PI);
	if (mlx_is_key_down(env->mlx, MLX_KEY_D)){
		env->pa += 0.015;
		draw_fov(&env);
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_A)){
		env->pa -= 0.015;
		draw_fov(&env);
	}	
}

void change_angles(t_env **d_env)
{
	t_env *env;
	double a_x;
	double a_y;
	
	env = *d_env;
	env->pa = fix_angle(env->pa);
	env->dx = cos(env->pa) * 2;
	env->dy = sin(env->pa) * 2;
	a_x = fix_angle(env->pa - 1.5708);
	a_y = fix_angle(env->pa + 1.5708);
	env->plane_x = cos(a_x);
	env->plane_y = sin(a_y);	
}
void	hook(void *param)
{
	t_env *env;
	env = param;
	
	if (mlx_is_key_down(env->mlx, MLX_KEY_D)){
		env->pa += 0.015;
		//dprintf(2, " DX:  %f,  DY:  :%f\n", env->dx, env->dy);
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_A))
	{
		env->pa -= 0.015;
		//dprintf(2, " DX:  %f,  DY:  :%f\n", env->dx, env->dy);
		
	}
	change_angles(&env);
	if (mlx_is_key_down(env->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(env->mlx);
	if (mlx_is_key_down(env->mlx, MLX_KEY_UP))
	{
		if (env->map[(int)((int)env->y - (int)env->plane_y - 2)/ 64][(int)(env->x -  2)/ 64] == '1')
		{
			env->y = env->y - env->dy;
		}
		else if (env->map[(int)(env->y)/ 64][(int)(env->x + (int)env->plane_x + 5)/ 64] == '1')
		{
			env->x = env->x + env->dx;
			dprintf(2, "COLISOION UP\n");
		}
		else{
			env->x = env->x + env->dx;
			env->y = env->y - env->dy;
		}
		dprintf(2, " <<<  Y  >>: %d\n <<  X  >>: %d\n",(int) env->y, (int)env->x);
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_DOWN))
	{
		if (env->map[(int)(env->y + (int)env->plane_y)/ 64][(int)(env->x)/ 64] == '1')
		{
			env->y = env->y + env->dy;
			dprintf(2, "COLISOION DOWN\n");
			return;
		}
		if (env->map[(int)(env->y)/ 64][(int)(env->x - (int)env->plane_x)/ 64] == '1')
		{
			dprintf(2, "COLISOION DOWN\n");
			return;
		}
			env->x = env->x - env->dx;
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_LEFT))
	{
		if (env->map[(int)(env->y + env->plane_y)/ 64][(int)(env->x)/ 64] == '1')
		{
			dprintf(2, "COLISOION LEFT\n");
			return;
		}
			env->y = env->y + env->plane_y ;
		if (env->map[(int)(env->y)/ 64][(int)(env->x + env->plane_x)/ 64] == '1')
		{
			dprintf(2, "COLISOION LEFT\n");
			return;
		}
			env->x = env->x + env->plane_x ;
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_RIGHT))
	{
			if (env->map[(int)(env->y - env->plane_y)/ 64][(int)(env->x)/ 64] == '1')
			{
				dprintf(2, "COLISOION RIGHT\n");
				return;
			}
			env->y = env->y - env->plane_y;
			if (env->map[(int)(env->y)/ 64][(int)(env->x - env->plane_x)/ 64] == '1')
			{
				dprintf(2, "COLISOION RIGHT\n");
				return;
			}
			env->x = env->x - env->plane_x;
	}
	draw_fov(&env);
}
int main(int argc, char **argv)
{
	t_env env;
	mlx_texture_t *texture;
	mlx_image_t *yellow;
	
	if (argc == 1)
		return (0);
	env.width = 0;
	env.height = 0;
	env.pa = PI  / 2;
	env.dx = cos(env.pa);
	env.dy = sin(env.pa);
	env.map = read_map(argv[1], &env.width, &env.height);
	env.top_x = env.width * 64;
	env.top_y = env.height * 64;
	env.dplane = 128 / tan(0.523599);
	printf("%i %i\n", env.width, env.height);
	env.mlx = mlx_init(1280, 800 , "MLX42", true);
	env.texture = mlx_load_png("./images/yellow.png");
	env.walls = mlx_texture_to_image(env.mlx, env.texture);
	env.found = mlx_new_image(env.mlx, env.width * WIDTH, env.height * HEIGHT);
	//env.player = mlx_new_image(env.mlx, 10, 10);
	//memset(env.player->pixels, 255, env.player->width * env.player->height * sizeof(int));
	//memset(env.found->pixels, 100, env.found->width * env.found->height * sizeof(int));
	mlx_image_to_window(env.mlx, env.found, 0, 0);
	fill_window(64, &env);
	mlx_loop_hook(env.mlx, &hook, &env);
	//mlx_loop_hook(env.mlx, &vortice_hook, &env);
	mlx_loop(env.mlx);
	mlx_terminate(env.mlx);
	return 0;
}	