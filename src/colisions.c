/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colisions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatute- <jmatute-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 12:05:48 by jmatute-          #+#    #+#             */
/*   Updated: 2023/01/01 14:52:07 by jmatute-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cub3d.h"

void X_colision(t_clcord *cord, double angle, t_env *env, int limit)
{
	double atan;

	atan = tan(angle);
	if (PI > angle)
	{
		cord->sub_y = - env->hpb;
		cord->yf = (((int)env->y >> 8) << 8) - 0.00001;
	}
	else if (PI < angle)
	{
		cord->sub_y = env->hpb;
		cord->yf = (((int)env->y >> 8) << 8) + env->hpb;
	}
	cord->sub_x = - cord->sub_y / atan;
	cord->xf = env->x + ((env->y - cord->yf) / atan);
	while (cord->xf < env->top_x && cord->yf < env->top_y && cord->xf  > 0 && cord->yf > 0)
	{
		if (env->map[(int)cord->yf / env->hpb][(int)cord->xf / env->hpb] == '1')
			break;
		cord->xf += cord->sub_x;
		cord->yf += cord->sub_y;
	}
	cord->abs = fabs((env->x - cord->xf) / cos(angle)) * cos(angle - env->pa);
	cord->xf = round(cord->xf);
}

void Y_colision(t_clcord *cord, double angle, t_env *env, int limit)
{
	double atan;

	atan = tan(angle);
	if (angle > P2 && angle < P3)
	{
		cord->sub_x = - env->hpb;
		cord->xf = (((int)env->x >> 8) << 8) - 0.00001;
	}
	else if (angle < P2 || angle > P3)
	{
		cord->sub_x = env->hpb;
		cord->xf = (((int)env->x >> 8) << 8) + env->hpb;
	}
	cord->sub_y = - cord->sub_x * atan;
	cord->yf = env->y + ((env->x - cord->xf) * atan);
	while(cord->xf < env->top_x  && cord->yf < env->top_y && cord->xf  > 0 && cord->yf > 0)
	{
		if (env->map[(int)cord->yf / env->hpb][(int)cord->xf / env->hpb] == '1')
			break;
		cord->xf += cord->sub_x;
		cord->yf += cord->sub_y;
	}
	cord->abs = fabs((env->y  - cord->yf) / sin(angle)) * cos(angle - env->pa);
	cord->yf = (int)cord->yf;
}

void select_texture(double angle, char axis, t_env **d_env)
{
	if (axis == 'x')
	{
		if (angle < PI)
			(*d_env)->texture = (*d_env)->tex->t_no;
		else
			(*d_env)->texture = (*d_env)->tex->t_so;
	}
	else if (axis == 'y')
	{
		if (angle > P2  && angle < P3)
			(*d_env)->texture = (*d_env)->tex->t_we;
		else
			(*d_env)->texture = (*d_env)->tex->t_ea;
	}	
}

void draw_column(t_env **d_env, double abs, double cord, int x)
{
	t_env			*env;
	mlx_texture_t	*text;
	double			height;
	int				y_i;
	
	env = *d_env;
	height = (env->hpb / abs ) * env->dplane;
	y_i = ((int)env->win_height >> 1) - ((int)height >> 1);
	text = get_text_column(env->texture, \
	get_number_column(floor(cord), env->texture), height, env);
	if (y_i < env->win_height && y_i > 0 && y_i + height < env->win_height)
	{
		dda_line(x,0,x,y_i,env->found,env->roof);
		mlx_draw_texture(env->found,text,x,y_i);
		dda_line(x,y_i + height,x,env->win_height - 1,env->found,env->floor);
	}
	else
		mlx_draw_texture(env->found,text,x,0);
	free(text);
}

int draw_colision(t_env **d_env, double angle, int x)
{
	t_env	*env;
	double	y;
	t_clcord absc;
	t_clcord ord;
	mlx_texture_t *text;
	
	env = (*d_env);
	X_colision(&absc, angle, env, x);
	Y_colision(&ord, angle, env, x);
	if ( absc.abs > ord.abs)
	{
		select_texture(angle, 'y', d_env);
		draw_column(d_env,ord.abs, ord.yf, x);
	}
	else
	{
		select_texture(angle, 'x', d_env);
		draw_column(d_env,absc.abs,absc.xf, x);
	}
	return (0);
}

void draw_fov(t_env **d_env)
{
	t_env	*env;
	double	angle;
	int		i;

	i = 0;
	env = *d_env;
	angle = env->pa - (RADIAN * 30);
	mlx_delete_image(env->mlx, env->found);
	env->found = mlx_new_image(env->mlx, env->win_width, env->win_height);
	mlx_image_to_window(env->mlx, env->found, 0, 0);
	while (i < env->win_width)
	{
		angle = fix_angle(angle);
		draw_colision(d_env, angle, i);
		angle += env->inc;
		i++;
	}
}