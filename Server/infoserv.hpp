/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infoserv.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:16:42 by rarraji           #+#    #+#             */
/*   Updated: 2024/04/26 11:13:59 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "map"
#include "request.hpp"

// class Request;

class infoserv
{
  public:
    std::string body;
    Request request;
};