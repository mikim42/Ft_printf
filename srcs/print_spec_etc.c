/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_spec_etc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikim <mikim@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/12 21:37:17 by mikim             #+#    #+#             */
/*   Updated: 2017/04/26 17:43:20 by mikim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	spec_time(t_env *e)
{
	time_t		t;
	struct tm	*tm;

	t = time(NULL);
	tm = localtime(&t);
	e->out = asctime(tm);
	e->ret += write(e->fd, e->out, ft_strlen(e->out));
	e->i++;
}

void	spec_ptraddr(t_env *e, char type)
{
	long long			tmp;
	unsigned long long	addr;

	init_int_arg(e, &tmp);
	addr = (unsigned long long)tmp;
	e->out = ft_lltoa_base(addr, 16);
	print_ptraddr(e, type);
}

void	spec_wchar(t_env *e, char type)
{
	wchar_t *ws;
	wchar_t wc[2];

	e->flag.minus ? e->flag.zero = 0 : 0;
	if (type == 's' || type == 'S')
	{
		init_wstr_arg(e, &ws);
		if (ws == NULL)
			return (print_null(e));
		print_wchar(e, ws);
	}
	else if (type == 'c' || type == 'C')
	{
		init_wchar_arg(e, &wc[0]);
		if (wc[0] == 0)
			return (print_null_char(e));
		wc[1] = 0;
		print_wchar(e, wc);
	}
}

void	spec_return(t_env *e)
{
	int	*tmp;

	if (e->tags.tag == pos)
	{
		va_copy(e->ap[0], e->ap[1]);
		while (--e->tags.pos >= 0)
			tmp = va_arg(e->ap[0], int *);
	}
	tmp = va_arg(e->ap[0], int *);
	*tmp = e->ret;
}

void	spec_percent(t_env *e)
{
	if (e->flag.minus)
	{
		e->ret += write(e->fd, "%", 1);
		while (e->flag.width-- > 1)
			e->ret += write(e->fd, " ", 1);
	}
	else
	{
		while (e->flag.width-- > 1)
			e->ret += (e->flag.zero ?
			write(e->fd, "0", 1) : write(e->fd, " ", 1));
		e->ret += write(e->fd, "%", 1);
	}
	e->i++;
}
