/* ups_build_in_rtos
 * /ups_build_in_rtos/gl/inc/fonts.hpp
 * fonts.hpp
 * 
 *  Created on: 14 дек. 2021 г.
 *  	  Time: 14:37:15
 *      Author: AlexICMT
 *
 * History:
 *
 * @file fonts.hpp
 * @date 13.07.2015
 * @author vlunyov
 * @brief Fonts for the UPS
 *
 * Last modified by: $Author: vlunyov $
 * $Revision: 1540 $
 * $Date: 2017-02-25 19:12:39 +0300 (Сб, 25 фев 2017) $
 *
 */

#ifndef GL_INC_FONTS_HPP_
#define GL_INC_FONTS_HPP_

#include "font.hpp"

/*
   |  0 1 2 3 4 5 6 7 8 9 A B C D E F
-------------------------------------
00 |              {escape}
10 |              {escape}
20 |    ! " # $ % & ' ( ) * + , - . /
30 |  0 1 2 3 4 5 6 7 8 9 : ; < = > ?
40 |  @ A B C D E F G H I J K L M N O
50 |  P Q R S T U V W X Y Z [ \ ] ^ _
60 |  ` a b c d e f g h i j k l m n o
70 |  p q r s t u v w x y z { | } ~ 
80 |              {icons}
90 |              {icons}
A0 |              {icons}
B0 |              {icons}
C0 |  А Б В Г Д Е Ж З И Й К Л М Н О П
D0 |  Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я
E0 |  а б в г д е ж з и й к л м н о п
F0 |  р с т у ф х ц ч ш щ ъ ы ь э ю я
*/

namespace kip5 {
namespace resources {

extern gl::Font font8n_min;

} // namespace resources
} // namespace kip5



#endif /* GL_INC_FONTS_HPP_ */
