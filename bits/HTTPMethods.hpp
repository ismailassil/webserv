/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMethods.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassil <iassil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 20:11:37 by iassil            #+#    #+#             */
/*   Updated: 2024/12/16 09:07:56 by iassil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class HTTPMethods {
	public:
		~HTTPMethods() {}
		virtual void	execute() = 0;
};

class GETMethod : public HTTPMethods {
	public:
		void execute();
};

class POSTMethod : public HTTPMethods {
	public:
		void execute();
};

class DELETEMethod : public HTTPMethods {
	public:
		void execute();
};