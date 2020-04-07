/* Copyright (C) 2014-2016 Achim Turan, mail@achim-turan.de
 * https://github.com/AchimTuran/asplib
 *
 * This file is part of asplib (Achim's Signal Processing LIBrary)
 *
 * asplib (Achim's Signal Processing LIBrary) is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * asplib (Achim's Signal Processing LIBrary) is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#include "Convolution/asplib_ConvolutionFactory.hpp"
#include "Convolution/Common/asplib_ConvolutionProducts.hpp"

using namespace std;


namespace asplib
{
const string CConvolutionFactoryMetaData::Name = "CConvolutionFactory";


CConvolutionFactory::CConvolutionFactory()
{
}


CConvolutionFactory::~CConvolutionFactory()
{
}

template class TAutoFactory<IConvolution, ConvolutionID_t, asplibFmt_t, CConvolutionFactoryMetaData>;
}


