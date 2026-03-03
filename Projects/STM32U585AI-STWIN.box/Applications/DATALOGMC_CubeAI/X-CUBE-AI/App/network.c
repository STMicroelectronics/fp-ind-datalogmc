/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-09-04T16:08:09+0000
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"


#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network

#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "0xb4fcb70884f0cdd13fa6b341daae3f4c"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2025-09-04T16:08:09+0000"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;


/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_input_layer_30_output_array, AI_ARRAY_FORMAT_FLOAT | AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4096, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  conversion_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4096, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 20420, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  pool_5_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 10200, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4056, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  pool_11_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2024, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2000, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  pool_17_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1000, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_20_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 976, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  pool_23_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 488, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  gemm_25_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  gemm_26_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  nl_27_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  conversion_28_output_array, AI_ARRAY_FORMAT_FLOAT | AI_FMT_FLAG_IS_IO,
  NULL, NULL, 2, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 320, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 20, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 640, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 256, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_20_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 256, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_20_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  gemm_25_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1952, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  gemm_25_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 4, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  gemm_26_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  gemm_26_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 2, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_2_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 984, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1712, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 752, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_20_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 752, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  gemm_25_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 508, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  gemm_26_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 14, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  nl_27_scratch0_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 256, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_14_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.004810516722500324f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_14_weights_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.0033228255342692137f, 0.002555434126406908f, 0.0028223677072674036f, 0.002690685912966728f,
                                                   0.003145163645967841f, 0.0028121250215917826f, 0.0026171565987169743f, 0.0026805177330970764f),
                                AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_20_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.007867127656936646f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_20_weights_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.003024383680894971f, 0.0030371593311429024f, 0.0022933846339583397f, 0.0030525734182447195f,
                                                   0.0028456670697778463f, 0.002879947191104293f, 0.0027158057782799006f, 0.00335007649846375f),
                                AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_2_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.005351372994482517f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_2_weights_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 20,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.0020115599036216736f, 0.0019739277195185423f, 0.0015643849037587643f, 0.002320504281669855f,
                                                   0.0031311458442360163f, 0.002342861844226718f, 0.002809974132105708f, 0.0021839323453605175f, 0.0016856837319210172f,
                                                   0.0017667454667389393f, 0.001787371002137661f, 0.002644316293299198f, 0.0015593832358717918f, 0.002878264058381319f,
                                                   0.002025363501161337f, 0.0026029301807284355f, 0.0021916229743510485f, 0.002403215505182743f, 0.0023800870403647423f,
                                                   0.0016838667215779424f),
                                AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_8_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.005510462448000908f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_8_weights_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.0023879713844507933f, 0.0028071438428014517f, 0.0019245361909270287f, 0.002109703840687871f,
                                                   0.0021381911356002092f, 0.002743813442066312f, 0.003068123711273074f, 0.002152431523427367f),
                                AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conversion_0_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.00392142403870821f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_25_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.06891175359487534f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_25_weights_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 4,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.0024194198194891214f, 0.0026907729916274548f, 0.0022170376032590866f, 0.002397258998826146f),
                                AI_PACK_INTQ_ZP(0, 0, 0, 0)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_26_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.028230365365743637f),
                                AI_PACK_INTQ_ZP(15)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_26_weights_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 2,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.00506126694381237f, 0.008389487862586975f),
                                AI_PACK_INTQ_ZP(0, 0)))

/* Int quant #13 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_27_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.00390625f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #14 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_11_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.005510462448000908f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #15 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_17_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.004810516722500324f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #16 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_23_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.007867127656936646f),
                                AI_PACK_INTQ_ZP(-128)))

/* Int quant #17 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_5_output_array_intq, AI_STATIC_CONST,
                              AI_BUFFER_META_FLAG_SCALE_FLOAT | AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
                              AI_PACK_INTQ_INFO(
                                AI_PACK_INTQ_SCALE(0.005351372994482517f),
                                AI_PACK_INTQ_ZP(-128)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_bias, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_14_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_output, AI_STATIC,
  1, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 250, 1), AI_STRIDE_INIT(4, 1, 1, 8, 2000),
  1, &conv2d_14_output_array, &conv2d_14_output_array_intq)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_scratch0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 752, 1, 1), AI_STRIDE_INIT(4, 1, 1, 752, 752),
  1, &conv2d_14_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_weights, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 8, 4, 1, 8), AI_STRIDE_INIT(4, 1, 8, 64, 256),
  1, &conv2d_14_weights_array, &conv2d_14_weights_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_20_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_20_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_20_output, AI_STATIC,
  5, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 122, 1), AI_STRIDE_INIT(4, 1, 1, 8, 976),
  1, &conv2d_20_output_array, &conv2d_20_output_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_20_scratch0, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 752, 1, 1), AI_STRIDE_INIT(4, 1, 1, 752, 752),
  1, &conv2d_20_scratch0_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_20_weights, AI_STATIC,
  7, 0x1,
  AI_SHAPE_INIT(4, 8, 4, 1, 8), AI_STRIDE_INIT(4, 1, 8, 64, 256),
  1, &conv2d_20_weights_array, &conv2d_20_weights_array_intq)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_bias, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 20, 1, 1), AI_STRIDE_INIT(4, 4, 4, 80, 80),
  1, &conv2d_2_bias_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_output, AI_STATIC,
  9, 0x1,
  AI_SHAPE_INIT(4, 1, 20, 1021, 1), AI_STRIDE_INIT(4, 1, 1, 20, 20420),
  1, &conv2d_2_output_array, &conv2d_2_output_array_intq)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_scratch0, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 984, 1, 1), AI_STRIDE_INIT(4, 1, 1, 984, 984),
  1, &conv2d_2_scratch0_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_2_weights, AI_STATIC,
  11, 0x1,
  AI_SHAPE_INIT(4, 4, 4, 1, 20), AI_STRIDE_INIT(4, 1, 4, 80, 320),
  1, &conv2d_2_weights_array, &conv2d_2_weights_array_intq)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_bias, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_8_bias_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_output, AI_STATIC,
  13, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 507, 1), AI_STRIDE_INIT(4, 1, 1, 8, 4056),
  1, &conv2d_8_output_array, &conv2d_8_output_array_intq)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_scratch0, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 1712, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1712, 1712),
  1, &conv2d_8_scratch0_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_weights, AI_STATIC,
  15, 0x1,
  AI_SHAPE_INIT(4, 20, 4, 1, 8), AI_STRIDE_INIT(4, 1, 20, 160, 640),
  1, &conv2d_8_weights_array, &conv2d_8_weights_array_intq)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  conversion_0_output, AI_STATIC,
  16, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1024), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &conversion_0_output_array, &conversion_0_output_array_intq)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  conversion_0_output0, AI_STATIC,
  17, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1024, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4096),
  1, &conversion_0_output_array, &conversion_0_output_array_intq)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  conversion_28_output, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &conversion_28_output_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  gemm_25_bias, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &gemm_25_bias_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  gemm_25_output, AI_STATIC,
  20, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &gemm_25_output_array, &gemm_25_output_array_intq)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  gemm_25_scratch0, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 508, 1, 1), AI_STRIDE_INIT(4, 2, 2, 1016, 1016),
  1, &gemm_25_scratch0_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  gemm_25_weights, AI_STATIC,
  22, 0x1,
  AI_SHAPE_INIT(4, 488, 4, 1, 1), AI_STRIDE_INIT(4, 1, 488, 1952, 1952),
  1, &gemm_25_weights_array, &gemm_25_weights_array_intq)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  gemm_26_bias, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &gemm_26_bias_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  gemm_26_output, AI_STATIC,
  24, 0x1,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &gemm_26_output_array, &gemm_26_output_array_intq)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  gemm_26_scratch0, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 14, 1, 1), AI_STRIDE_INIT(4, 2, 2, 28, 28),
  1, &gemm_26_scratch0_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  gemm_26_weights, AI_STATIC,
  26, 0x1,
  AI_SHAPE_INIT(4, 4, 2, 1, 1), AI_STRIDE_INIT(4, 1, 4, 8, 8),
  1, &gemm_26_weights_array, &gemm_26_weights_array_intq)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  nl_27_output, AI_STATIC,
  27, 0x1,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &nl_27_output_array, &nl_27_output_array_intq)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  nl_27_scratch0, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &nl_27_scratch0_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  pool_11_output, AI_STATIC,
  29, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 253, 1), AI_STRIDE_INIT(4, 1, 1, 8, 2024),
  1, &pool_11_output_array, &pool_11_output_array_intq)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  pool_17_output, AI_STATIC,
  30, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 125, 1), AI_STRIDE_INIT(4, 1, 1, 8, 1000),
  1, &pool_17_output_array, &pool_17_output_array_intq)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  pool_23_output, AI_STATIC,
  31, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 61, 1), AI_STRIDE_INIT(4, 1, 1, 8, 488),
  1, &pool_23_output_array, &pool_23_output_array_intq)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  pool_23_output0, AI_STATIC,
  32, 0x1,
  AI_SHAPE_INIT(4, 1, 488, 1, 1), AI_STRIDE_INIT(4, 1, 1, 488, 488),
  1, &pool_23_output_array, &pool_23_output_array_intq)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  pool_5_output, AI_STATIC,
  33, 0x1,
  AI_SHAPE_INIT(4, 1, 20, 510, 1), AI_STRIDE_INIT(4, 1, 1, 20, 10200),
  1, &pool_5_output_array, &pool_5_output_array_intq)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_input_layer_30_output, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1024), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &serving_default_input_layer_30_output_array, NULL)


/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conversion_28_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_27_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_28_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conversion_28_layer, 28,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conversion_28_chain,
  NULL, &conversion_28_layer, AI_STATIC,
)


AI_STATIC_CONST ai_i32 nl_27_nl_params_data[] = { 1939975936, 21, -992 };
AI_ARRAY_OBJ_DECLARE(
  nl_27_nl_params, AI_ARRAY_FORMAT_S32,
  nl_27_nl_params_data, nl_27_nl_params_data, 3, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_27_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_26_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_27_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_27_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  nl_27_layer, 27,
  SM_TYPE, 0x0, NULL,
  sm, forward_sm_integer,
  &nl_27_chain,
  NULL, &conversion_28_layer, AI_STATIC,
  .nl_params = &nl_27_nl_params,
  .axis = AI_SHAPE_CHANNEL,
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_26_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_25_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_26_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_26_weights, &gemm_26_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_26_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_26_layer, 26,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_26_chain,
  NULL, &nl_27_layer, AI_STATIC,
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_25_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_23_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_25_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_25_weights, &gemm_25_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_25_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_25_layer, 25,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_25_chain,
  NULL, &gemm_26_layer, AI_STATIC,
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_23_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_20_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_23_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_23_layer, 23,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp_integer_INT8,
  &pool_23_chain,
  NULL, &gemm_25_layer, AI_STATIC,
  .pool_size = AI_SHAPE_2D_INIT(2, 1),
  .pool_stride = AI_SHAPE_2D_INIT(2, 1),
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_20_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_17_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_20_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_20_weights, &conv2d_20_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_20_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_20_layer, 20,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_20_chain,
  NULL, &pool_23_layer, AI_STATIC,
  .groups = 1,
  .filter_stride = AI_SHAPE_2D_INIT(1, 1),
  .dilation = AI_SHAPE_2D_INIT(1, 1),
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_17_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_14_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_17_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_17_layer, 17,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp_integer_INT8,
  &pool_17_chain,
  NULL, &conv2d_20_layer, AI_STATIC,
  .pool_size = AI_SHAPE_2D_INIT(2, 1),
  .pool_stride = AI_SHAPE_2D_INIT(2, 1),
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_14_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_11_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_14_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_14_weights, &conv2d_14_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_14_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_14_layer, 14,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_14_chain,
  NULL, &pool_17_layer, AI_STATIC,
  .groups = 1,
  .filter_stride = AI_SHAPE_2D_INIT(1, 1),
  .dilation = AI_SHAPE_2D_INIT(1, 1),
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_11_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_11_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_11_layer, 11,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp_integer_INT8,
  &pool_11_chain,
  NULL, &conv2d_14_layer, AI_STATIC,
  .pool_size = AI_SHAPE_2D_INIT(2, 1),
  .pool_stride = AI_SHAPE_2D_INIT(2, 1),
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_8_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_8_weights, &conv2d_8_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_8_layer, 8,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_deep_sssa8_ch,
  &conv2d_8_chain,
  NULL, &pool_11_layer, AI_STATIC,
  .groups = 1,
  .filter_stride = AI_SHAPE_2D_INIT(1, 1),
  .dilation = AI_SHAPE_2D_INIT(1, 1),
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_5_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_5_layer, 5,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp_integer_INT8,
  &pool_5_chain,
  NULL, &conv2d_8_layer, AI_STATIC,
  .pool_size = AI_SHAPE_2D_INIT(2, 1),
  .pool_stride = AI_SHAPE_2D_INIT(2, 1),
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_0_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_2_weights, &conv2d_2_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_2_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_2_layer, 2,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_2_chain,
  NULL, &pool_5_layer, AI_STATIC,
  .groups = 1,
  .filter_stride = AI_SHAPE_2D_INIT(1, 1),
  .dilation = AI_SHAPE_2D_INIT(1, 1),
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0),
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID,
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conversion_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_input_layer_30_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conversion_0_layer, 0,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conversion_0_chain,
  NULL, &conv2d_2_layer, AI_STATIC,
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
                 AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3632, 1, 1),
                 3632, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
                 AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 25500, 1, 1),
                 25500, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_input_layer_30_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &conversion_28_output),
  &conversion_0_layer, 0x45e11dc6, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
    AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
                   AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 3632, 1, 1),
                   3632, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
    AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
                   AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 25500, 1, 1),
                   25500, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_input_layer_30_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &conversion_28_output),
  &conversion_0_layer, 0x45e11dc6, NULL)

#endif  /*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network *net_ctx, const ai_network_params *params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params))
  {
    /* Updating activations (byte) offsets */

    serving_default_input_layer_30_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    serving_default_input_layer_30_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    conversion_0_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    conversion_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    conv2d_2_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 4096);
    conv2d_2_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 4096);
    conv2d_2_output_array.data = AI_PTR(g_network_activations_map[0] + 5080);
    conv2d_2_output_array.data_start = AI_PTR(g_network_activations_map[0] + 5080);
    pool_5_output_array.data = AI_PTR(g_network_activations_map[0] + 5080);
    pool_5_output_array.data_start = AI_PTR(g_network_activations_map[0] + 5080);
    conv2d_8_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 0);
    conv2d_8_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    conv2d_8_output_array.data = AI_PTR(g_network_activations_map[0] + 15280);
    conv2d_8_output_array.data_start = AI_PTR(g_network_activations_map[0] + 15280);
    pool_11_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    pool_11_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    conv2d_14_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 2024);
    conv2d_14_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 2024);
    conv2d_14_output_array.data = AI_PTR(g_network_activations_map[0] + 2776);
    conv2d_14_output_array.data_start = AI_PTR(g_network_activations_map[0] + 2776);
    pool_17_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    pool_17_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    conv2d_20_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 1000);
    conv2d_20_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 1000);
    conv2d_20_output_array.data = AI_PTR(g_network_activations_map[0] + 1752);
    conv2d_20_output_array.data_start = AI_PTR(g_network_activations_map[0] + 1752);
    pool_23_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    pool_23_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    gemm_25_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 488);
    gemm_25_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 488);
    gemm_25_output_array.data = AI_PTR(g_network_activations_map[0] + 1504);
    gemm_25_output_array.data_start = AI_PTR(g_network_activations_map[0] + 1504);
    gemm_26_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 0);
    gemm_26_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    gemm_26_output_array.data = AI_PTR(g_network_activations_map[0] + 28);
    gemm_26_output_array.data_start = AI_PTR(g_network_activations_map[0] + 28);
    nl_27_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 32);
    nl_27_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 32);
    nl_27_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    nl_27_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    conversion_28_output_array.data = AI_PTR(g_network_activations_map[0] + 4);
    conversion_28_output_array.data_start = AI_PTR(g_network_activations_map[0] + 4);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network *net_ctx, const ai_network_params *params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params))
  {
    /* Updating weights (byte) offsets */

    conv2d_2_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_2_weights_array.data = AI_PTR(g_network_weights_map[0] + 0);
    conv2d_2_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    conv2d_2_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_2_bias_array.data = AI_PTR(g_network_weights_map[0] + 320);
    conv2d_2_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 320);
    conv2d_8_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_8_weights_array.data = AI_PTR(g_network_weights_map[0] + 400);
    conv2d_8_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 400);
    conv2d_8_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_8_bias_array.data = AI_PTR(g_network_weights_map[0] + 1040);
    conv2d_8_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 1040);
    conv2d_14_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_14_weights_array.data = AI_PTR(g_network_weights_map[0] + 1072);
    conv2d_14_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 1072);
    conv2d_14_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_14_bias_array.data = AI_PTR(g_network_weights_map[0] + 1328);
    conv2d_14_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 1328);
    conv2d_20_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_20_weights_array.data = AI_PTR(g_network_weights_map[0] + 1360);
    conv2d_20_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 1360);
    conv2d_20_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_20_bias_array.data = AI_PTR(g_network_weights_map[0] + 1616);
    conv2d_20_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 1616);
    gemm_25_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_25_weights_array.data = AI_PTR(g_network_weights_map[0] + 1648);
    gemm_25_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 1648);
    gemm_25_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_25_bias_array.data = AI_PTR(g_network_weights_map[0] + 3600);
    gemm_25_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 3600);
    gemm_26_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_26_weights_array.data = AI_PTR(g_network_weights_map[0] + 3616);
    gemm_26_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 3616);
    gemm_26_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_26_bias_array.data = AI_PTR(g_network_weights_map[0] + 3624);
    gemm_26_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 3624);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/


AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report *report)
{
  ai_network *net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r =
    {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,

      .compile_datetime  = AI_TOOLS_COMPILE_TIME,

      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {
        AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
        AI_TOOLS_VERSION_MICRO, 0x0
      },
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),

      .n_macc            = 784092,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x45e11dc6,
    };

    if (!ai_platform_api_get_network_report(network, &r)) { return false; }

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report *report)
{
  ai_network *net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r =
    {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,

      .compile_datetime  = AI_TOOLS_COMPILE_TIME,

      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {
        AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
        AI_TOOLS_VERSION_MICRO, 0x0
      },
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),

      .n_macc            = 784092,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x45e11dc6,
    };

    if (!ai_platform_api_get_network_report(network, &r)) { return false; }

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_network_create(
  ai_handle *network, const ai_buffer *network_config)
{
  return ai_platform_network_create(
           network, network_config,
           AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
           AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle *network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE)
  {
    return err;
  }

  if (ai_network_data_params_get(&params) != true)
  {
    err = ai_network_get_error(*network);
    return err;
  }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx = 0; activations && idx < params.map_activations.size; idx++)
  {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx = 0; weights && idx < params.map_weights.size; idx++)
  {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_network_init(*network, &params) != true)
  {
    err = ai_network_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer *ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL)
  {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer *ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL)
  {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params *params)
{
  ai_network *net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) { return false; }
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer *input, ai_buffer *output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer *input)
{
  return ai_platform_network_process(network, input, NULL);
}


#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

