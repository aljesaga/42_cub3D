/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatute- <jmatute-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 18:32:27 by jmatute-          #+#    #+#             */
/*   Updated: 2022/11/06 22:14:20 by jmatute-         ###   ########.fr       */
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
	while(i <= dcords.p){
		 mlx_put_pixel(flor, dcords.x, dcords.y, color);
		 dcords.x += dcords.incx;
		 dcords.y += dcords.incy;
		 i++;
	}
}

void draw_separator(t_env **d_env)
{
	t_env *env;
	int x;
	int y;
	
	env = *d_env;
	x = 0;
	y = 0;
	while ( x < env->width)
	{
		dda_line(x * WIDTH, 0 , x * WIDTH, env->height * HEIGHT - 1 , env->found,16711680);	
		x++;
	}
	while (y < env->height)
	{
		dda_line(0 , y * HEIGHT, env->width * WIDTH  - 1 , y *HEIGHT, env->found,16711680);
		y++;
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
				mlx_image_to_window(env->mlx, env->walls, x * l_image + 1 , y * l_image + 1);
			else if (env->map[y][x] == 'P'){
				mlx_image_to_window(env->mlx, env->player ,x *l_image ,y * l_image);
				env->x = x * l_image + 5;
				env->y = y * l_image + 5;
			}
			x++;
		}
		x = 0;
		y ++;
	}
	draw_separator(&env);
	return (0);
}
void player_frames(t_env **d_env){
	t_env	*env;
	float Xa, Ya;
	int Xnew, Ynew;
	int Xnew_y, Ynew_y;
	
	env = (*d_env);
	mlx_delete_image(env->mlx, env->found);
	env->found = mlx_new_image(env->mlx, env->width * WIDTH, env->height * HEIGHT);
	memset(env->found->pixels, 100, env->found->width * env->found->height * sizeof(int));
	mlx_image_to_window(env->mlx, env->found, 0, 0);
	draw_separator(&env);
	env->found->instances->z = env->walls->instances[0].z - 1;
	if (env->pa < PI)
	{
		Ya = 64;
		Ynew = floor((env->y >> 6) << 6);
	}
	else if (env->pa < PI){
		Ya = -64;
		Ynew = floor(((env->y >> 6) << 6)) + 64;
	}
	Xnew = env->x + (env->y - Ynew) / tan(env->pa);
	if (env->pa < P2 || env->pa > P3)
		Xnew_y =(env->x /64) * 64 + 64;
	else if (env->pa > P2 &&  env->pa < P3)
	Xa = 64 / tan(env->pa);
	dprintf(2,"BEFORE%d   %d", Xnew, Ynew);
	if(Xnew > env->width * 64){
		Xnew = env->width * 64 -1;
	}
	if (Ynew > env->height * 64){
		Ynew = env->height * 64 - 1;
	}
	if (Ynew < 0)
	{
		Ynew = 0;
	}
	if (Xnew < 0)
		Xnew = 0;
	
	while( Ynew > 0 && Ynew < env->height *64){
		if (env->map[(Ynew/64 )][(Xnew/64)] == '1')
			break;
		Ynew += Ya;
		Xnew += Xa;
	}
	dprintf(2,"DIVIDES %d   %d \n", (Xnew/64), (Ynew/64));
	dda_line(env->x , env->y, Xnew, Ynew, env->found, 116711872);
	// dda_line(env->x , env->y, env->x +(env->dx *10), env->y +(env->dy *10), env->found, 116711872);
	dprintf(2, " LENGTHS %d   %d\n", Xnew, Ynew);
}
void vortice_hook(void *param){
	t_env *env;
	env = param;
	
	if (mlx_is_key_down(env->mlx, MLX_KEY_A)){
		env->pa -= 0.05;
		if (env->pa < 0)
			env->pa += 2 *PI;
		env->dx = cos(env->pa) * 5;
		env->dy = sin(env->pa) * 5;
		player_frames(&env);
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_D)){
		env->pa += 0.05;
		if (env->pa > 2 *PI)
			env->pa -= 2 *PI;
		env->dx = cos(env->pa) * 5;
		env->dy = sin(env->pa) * 5;
		player_frames(&env);
	}	
}

void	hook(void *param)
{
	t_env *env;
	env = param;
	
	if (mlx_is_key_down(env->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(env->mlx);
	if (mlx_is_key_down(env->mlx, MLX_KEY_UP)){
		env->player->instances[0].y -= 5;
		env->y -= 5;
		player_frames(&env);
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_DOWN)){
		
		env->player->instances[0].y += 5;
		env->y += 5;
		player_frames(&env);
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_LEFT)){
		env->player->instances[0].x -= 5;
		env->x -= 5;
		player_frames(&env);
	}
	if (mlx_is_key_down(env->mlx, MLX_KEY_RIGHT)){
		env->player->instances[0].x += 5;
		env->x += 5;
		player_frames(&env);
	}
	
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
	env.pa = PI;
	env.dx = cos(env.pa) * 5;
	env.dy = sin(env.pa) * 5;
	env.map = read_map(argv[1], &env.width, &env.height);
	printf("%d %d\n", env.width, env.height);
	env.mlx = mlx_init(env.width * WIDTH , env.height * HEIGHT , "MLX42", true);
	env.texture = mlx_load_png("./images/yellow.png");
	env.walls = mlx_texture_to_image(env.mlx, env.texture);
	env.found = mlx_new_image(env.mlx, env.width * WIDTH, env.height * HEIGHT);
	env.player = mlx_new_image(env.mlx, 10, 10);
	memset(env.player->pixels, 255, env.player->width * env.player->height * sizeof(int));
	memset(env.found->pixels, 100, env.found->width * env.found->height * sizeof(int));
	mlx_image_to_window(env.mlx, env.found, 0, 0);
	fill_window(64, &env);
	mlx_loop_hook(env.mlx, &hook, &env);
	mlx_loop_hook(env.mlx, &vortice_hook, &env);
	mlx_loop(env.mlx);
	mlx_terminate(env.mlx);
	return 0;
}	