/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmatute- <jmatute-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 16:05:08 by jmatute-          #+#    #+#             */
/*   Updated: 2022/12/19 17:37:00 by jmatute-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cub3d.h"

mlx_texture_t* get_column(mlx_texture_t *texture, int column, int height)
{
	mlx_texture_t	*tex;
	double			step;		
	int				pos_pixel;		
	int				it;
	double			c_step;
	
	it = 0;
	step = 0;
	c_step =  (double)texture->height / (double)height;
	tex = malloc(sizeof(mlx_texture_t));
	tex->pixels = malloc(sizeof(uint8_t) * (height * 4));
	tex->bytes_per_pixel = 4;
	tex->height = height;
	tex->width = 1;
	while(it < (height * 4))
	{
		tex->pixels[it] = texture->pixels[(int)(step) * (texture->width* 4) + (column * 4)];
		tex->pixels[it + 1] =texture->pixels[(int)(step) *(texture->width * 4) + 1 + (column * 4)];
		tex->pixels[it + 2] =texture->pixels[(int)(step) *(texture->width * 4) + 2 + (column * 4)];
		tex->pixels[it + 3] =texture->pixels[(int)(step) *(texture->width * 4) + 3 + (column * 4)];
		step += c_step;
		it+=4;
	}
	return (tex);
}


int get_number_column(int cord, mlx_texture_t *texture)
{
	int result;

	result = (cord % 256);
	return(result);
}
