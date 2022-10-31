/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meudier <meudier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 21:38:09 by amahla            #+#    #+#             */
/*   Updated: 2022/10/31 11:24:47 by meudier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DEFINES_H__
# define __DEFINES_H__

# define DEBUG 0

# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define YELLOW "\033[0;33m"
# define SET "\033[0m"

enum e_method
{
    GET,
    POST,
    DELETE,
    BAD_REQUEST
};

#endif
