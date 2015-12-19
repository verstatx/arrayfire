/*******************************************************
 * Copyright (c) 2015, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <Array.hpp>

namespace cpu
{
namespace kernel
{

template<typename inType, typename outType, bool isLinear>
void histogram(Array<outType> out, Array<inType> const in,
               unsigned const nbins, double const minval, double const maxval)
{
    dim4 const outDims   = out.dims();
    float const step     = (maxval - minval)/(float)nbins;
    dim4 const inDims    = in.dims();
    dim4 const iStrides  = in.strides();
    dim4 const oStrides  = out.strides();
    dim_t const nElems   = inDims[0]*inDims[1];

    outType *outData    = out.get();
    const inType* inData= in.get();

    for(dim_t b3 = 0; b3 < outDims[3]; b3++) {
        for(dim_t b2 = 0; b2 < outDims[2]; b2++) {
            for(dim_t i=0; i<nElems; i++) {
                int idx = isLinear ? i : ((i % inDims[0]) + (i / inDims[0])*iStrides[1]);
                int bin = (int)((inData[idx] - minval) / step);
                bin = std::max(bin, 0);
                bin = std::min(bin, (int)(nbins - 1));
                outData[bin]++;
            }
            inData  += iStrides[2];
            outData += oStrides[2];
        }
    }
}

}
}
