/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-09-04T16:08:09+0000
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] =
{
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};


/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[454] =
{
  0xa00e672196f27fa9U, 0x11533891f5b1fd0U, 0x5224e243a25efa6cU, 0xac8142bb3aaf0961U,
  0x4ce45487d847cb1aU, 0xb88e2c6b5a7f097bU, 0xef017f37fc4715c9U, 0x38f793bcb8b3da3eU,
  0xed0208c6d9bae213U, 0xffb7c4fa0f4e7f07U, 0xd97ffc0862c32064U, 0x16ea3f4d0cd007fbU,
  0x384bbd23e0494f3aU, 0xfe7f4b4dfcbac094U, 0x7f4b1da237e6ead0U, 0x211b540bb283b53aU,
  0x1d727f67229c4ab6U, 0xa82a1947b0bb600cU, 0x3bed6e0f3694d659U, 0x7f16e60bab347677U,
  0x33d3c23e3017997fU, 0xa7a39cfbf0af33fU, 0xf25fec2f1ceae912U, 0x5a623d2a407f1f26U,
  0x6eb8f1a63bb16cf3U, 0x4881bee05687d359U, 0x27d1ef6dfc6f1fe8U, 0x2a646705bda081abU,
  0x4d77facc2a14b97fU, 0x100dca96a997d07dU, 0xf0c2a737fa7fe037U, 0x233a410e09e6a0baU,
  0x2e4e8126b812344U, 0x22ed0dbcad869054U, 0xffececbf7f270058U, 0xab07fd95e5303a16U,
  0x9fab81b334da6e09U, 0x6440d642e915bc10U, 0x6f9e7c05fa7fd85fU, 0x354274821edb2e90U,
  0xffffb9caffffe1e0U, 0x58bffff96f1U, 0xffffe67affffe09fU, 0xffffd4e3fffff9c1U,
  0xffffd5cfffffcc05U, 0xffffca9cffffc8a4U, 0xfffffa03ffffd798U, 0xffffc005ffffaafaU,
  0xffffba4a00002c00U, 0xffffd5fa000016a9U, 0x3c2a2d40c0c18cd5U, 0x551bada5522f41c5U,
  0xf5f2a88ff9983eeaU, 0x29100efdef25ee91U, 0xf08cead72b2236c9U, 0x18da332cab1bcfb8U,
  0xbca7d62a4f31cddcU, 0xdf58ccd0269cfd81U, 0x504c32c4d55bf5bbU, 0xd2e20c97c1da8738U,
  0xa169ba7f09c9b0e1U, 0xc7c3158c20a70f33U, 0x480007e03a2fd1f9U, 0xf7ec0e02a30ce05cU,
  0xd5efd3f758405697U, 0xa555e52f2c02bed6U, 0xe94a66a31ac59ef4U, 0xe4dcc805e015b50aU,
  0xea11d702e44f0238U, 0xc410f2c5482f38e4U, 0x479bcbb7c0accd82U, 0xded9992b0a0e2ff2U,
  0xd9f10f3f1bb8506U, 0x98308eff60004d96U, 0xe0e2d9b6334c639eU, 0x1ac1b2d691ebba02U,
  0x93d081c0423faf53U, 0xf0cc3d3a6d3b3c87U, 0xffedb72be19ebadaU, 0x1b35b51bfea244aeU,
  0x4f5f34d9f9e31eefU, 0x4d4eceae250b9a13U, 0x58ba71f198e4c70fU, 0xafa19bbadc4cdd9aU,
  0xf481dd92d4295ce7U, 0xc038b517be414bf4U, 0x2840c9cee9b4a404U, 0xcc517d9542c4bb2U,
  0x2fa8232657c2811aU, 0x4a285cc504609d60U, 0x1bc703f497b918cfU, 0xe0c5000334a6a8d7U,
  0xe1dde6bab8321836U, 0x2b7e99e1c3da5ddU, 0x2830fb0020500e2eU, 0x87fd583dd9231822U,
  0x58202740e6158124U, 0xdafe58e0e4f3252bU, 0xa522a9fd302d45f7U, 0x10cbd892d26717f5U,
  0xc8f1ba1dc48c10cbU, 0x4bcd44ccb20893a0U, 0xb1e2aeb64b0c0f1fU, 0xe319bf1bb7b81376U,
  0x1abed4520deb0fe1U, 0xb52b855bf89b6e1U, 0x4e3ec3790f31fa3U, 0x5aab32f6abde0207U,
  0xa481b0a41afaf032U, 0xf43e08441108120bU, 0xe64e43d627e3c2cfU, 0x4be57fe60500e511U,
  0x39cae5c83ce7afcbU, 0xdcf630fc1bdf11e8U, 0xe9fad2053acf3cdbU, 0xc8694c470a42aab3U,
  0xbef46cffef1ccd5U, 0x140bd2e203d306d5U, 0x13d3da410b4aeb1aU, 0x133cf2918291d21U,
  0x22600616b9e691edU, 0x62d1e6ae1f1350d7U, 0x105141522363d3b7U, 0x27b348d735f099e8U,
  0xee9dffb60a387a18U, 0xbac0854ca15153bU, 0x62090612b696cba6U, 0xd1da1c10dfc0f9a8U,
  0x24551c98e8492e01U, 0xc8c84781c935eea6U, 0xfffff013ffffde25U, 0xffffdd8efffff7e6U,
  0x59effffe436U, 0xffffe5aafffffd43U, 0xe5e1fa23df207f2cU, 0x243632f5c4a23a63U,
  0xf107db28e7347037U, 0xed492b323f422320U, 0x3210df58ebc0cb4dU, 0x66a0923f397afc1bU,
  0x598105293d1012dfU, 0xeb92edffdefbe204U, 0xcdd53ef1b80ab138U, 0xffa12f11fb205342U,
  0xa6690ac4eb1b0c15U, 0x37e04d3d97bc7f3eU, 0xf25d792bfc257ba0U, 0xf5e57f9ac3a54cadU,
  0x5c6066bcf6a95061U, 0x65462cf59d8df92dU, 0x434462a91cdd1eb1U, 0xc5f2f73205f07f44U,
  0x556aef3d1fa63f47U, 0x2a495d3253c0181bU, 0x419416e81c3b11eeU, 0xd481c90ea0f4d7c3U,
  0xa5b838d0101cf70cU, 0x362ba22fc9e021b5U, 0x79d2e2c238f5274bU, 0xd4c1e914a2ac5da3U,
  0x36dac9d64d471b53U, 0xfbe0b37fdab241b3U, 0x8f245ca5bfa47fc0U, 0x22d904c69d272672U,
  0xde2f300d2a335c59U, 0x24e547a28e2a575eU, 0x2862ffffedd2U, 0xffffecc80000321dU,
  0x19daffffe3e8U, 0xfffff34effffbafdU, 0x5acbb3f461ed1c56U, 0x7f961a2a7a36d9d6U,
  0x43cae7e3094eb429U, 0xce03166515f8fcU, 0xd6bcc290e8375655U, 0xa6f7d18236c15ba9U,
  0xc746e2bd25e842e9U, 0x4e7f4a10c154edfaU, 0x718a69ca9ea53c08U, 0xcf6ee55d7fd513b0U,
  0xe90763277043850eU, 0xbec24b5fa9c09f27U, 0xc57fdee2da333b31U, 0x9ae779cbbeb578acU,
  0xfc61cb3fdab33db2U, 0xfc1f26fcf23e0b0eU, 0xcaa41aaa9d6927c2U, 0xfce336bfb3eb19dbU,
  0xddb53dcad0642081U, 0x1d3c4c83dd130fa6U, 0x7abc3cb7021bda42U, 0x7f9ea36a462516c0U,
  0x56e1254a16f0f44aU, 0x3abaaf273b3cee3fU, 0xfbf9da1ee643ea3dU, 0x4e439558cffe9c43U,
  0x5bb450cd0c379d17U, 0xf8de81421d27dfdeU, 0x1008e1dad381fe92U, 0x22331c81a09dcaa2U,
  0x1536e3eef34b39dfU, 0x6168403026003c56U, 0xed8fffff2e7U, 0x2d5efffff211U,
  0xffffe5b400005a13U, 0xffffc8aefffff9f3U, 0x5b6c83e2bf2e6e5U, 0xc7ccd21a2ed005c7U,
  0xecb8c3601fcdeec2U, 0x3ff6ea30f8e4efe6U, 0x1be7e616f82edf0fU, 0x1c17f512f40a2af2U,
  0x5f0f85610d9f6d8U, 0xdf10f5a112444ffU, 0x6e1041233d728e2U, 0xd5c8b41917e2f6ddU,
  0xc7cdd23209e1ebe3U, 0xf2dbf32018d524d8U, 0x2610f5394a18faf2U, 0x405212830ee1317U,
  0x1ff8f03b2e22f7deU, 0x3015f844290e1de5U, 0x2aebf91518f1f2e0U, 0x2aeb242e40fbfaf9U,
  0x103d1062f1115dcU, 0x1ce5ed2a29fffb13U, 0x3a0b070620e422e3U, 0xf0cceb25eaea18e1U,
  0x201c27ed1bdc19e7U, 0xc19e6492519411bU, 0xf910163310f028f7U, 0x36ebfc1a1fec221eU,
  0xd034134c1b1e14f1U, 0xe5d8077f1ecbf405U, 0x11e0fb32e3c833e4U, 0x11e2125922fe18d1U,
  0x2024105af8002cf5U, 0x25e9f54732effaf1U, 0xfa13225a1ff01a0fU, 0x3e15011402e80826U,
  0x2e9db3b17f01b1dU, 0x1ae8124bfaf82dd4U, 0xedbe153122f21918U, 0x9d908f5f5f3f80dU,
  0x37f626283522f305U, 0x17f61a26fc082f03U, 0x1204006535022dfcU, 0x3d01153bfe0d00d9U,
  0x5e2fd6e220441f5U, 0xe5f9e53c0323fdd7U, 0xebfeda343ed7f8d9U, 0xf8f2d3471a0a2ae0U,
  0x1dc8f23e34f4ecc3U, 0xede235932321fefU, 0x9f6fb3040e801ceU, 0x160b06413bff04feU,
  0xd06e3591f1afedaU, 0x1cf7f63740331cfeU, 0xf316e15c25282501U, 0xddeff958f4e70ce0U,
  0xdde6e41832f233d5U, 0x18c8f12719bf08b6U, 0xfbd0ceec07d5e4caU, 0x6eaef3e391b06d3U,
  0xd1aeb2f35dff3f9U, 0x2f13e26f4eef0ff2U, 0xf0e209550fe924caU, 0x15f70935420e0aebU,
  0xf4d8d23b46f918e3U, 0xbc5cd0b030713dfU, 0xf5fac52438db26e6U, 0xe4e5c22434d5d9cdU,
  0xbaccd50ef1f7f9a0U, 0x29eef302231101d4U, 0x220d1e2a22e3280bU, 0x1105ff3511df19d4U,
  0x160efc4e4105fceaU, 0x206f26819faffe7U, 0x3df00a3009f53712U, 0x1316f65434152bd5U,
  0xea0ce34617df35d0U, 0xdff7fd7ffce51cd1U, 0x5fe0756280300e8U, 0x111a102539fbf106U,
  0x2cfbec1b4016fc00U, 0x1332dc0d092f24f8U, 0x38fa092af7e81a0dU, 0x39270f2ef5e416fcU,
  0xee1a1a1ff115faf5U, 0xddf5d23227bcec03U, 0xfed4fa01d8eff607U, 0x28fe023e0ef7fff6U,
  0xfe03023428f42900U, 0x2621e66b27191dfcU, 0x2e0de55e30191ad8U, 0x3dea23381a071c06U,
  0xf6f8f609f40031dcU, 0x2cebea21031e321bU, 0xf4f0226329012dffU, 0x23f71f1f09fe04ecU,
  0x280b0760280b24fdU, 0x1ef81e2e26001b22U, 0x2124fd441ff12d06U, 0xf31bf6fb0ff83714U,
  0xf9e405fb33fe11ebU, 0xf9f60806260022ffU, 0xe1d8c9e9f70ae6f5U, 0xe303ee1813d106dfU,
  0x2117d913e4e6fbf3U, 0x1e10b56270b29e7U, 0x170e174123ed29feU, 0xe411ff4f3cf0f6e9U,
  0xe4005e35edf7ffU, 0x8240d52370830fdU, 0xbe7f77116fc1bfbU, 0xe1d6c34f08c629d1U,
  0xf1e7d35d2dd52bc2U, 0x10f2d51e19fd2c02U, 0x6f3e056431514f3U, 0x3216e65a230524ecU,
  0xdb02f53e3a2934ddU, 0x2004010e4005e8e8U, 0xea00ef29fa19ffe7U, 0xddf4f028fccae8efU,
  0xbcece85d01c306b0U, 0x14f7df0305d214beU, 0xd0be75c21cbffbfU, 0x706e24f41e00004U,
  0xd02ba5e62ec0610U, 0xd4b7e24c4ddcedb7U, 0xc6dfd1f1fbd3d3aaU, 0xf1b0ab56f31afa9bU,
  0xe808bfef250edbe7U, 0x10d813d7f4ffea00U, 0x2df5261c13f9facbU, 0x2be8f87428e9dfd1U,
  0x1e08ef18180d0ee8U, 0xd2e5f87f14d822feU, 0xcfffb75308fb2b00U, 0x24f0ff29f10824acU,
  0xe3d8de5b04da3bc3U, 0x44c6e05b27f50fedU, 0x3c24e348f7defbd9U, 0xfc02f63f25fd160aU,
  0xb241c1e123037eeU, 0x16f10bf437f6ebU, 0x419c7f5fdf822d7U, 0xffffe4050ffafc0cU,
  0xe7d4d718eeda03e4U, 0x36d3131de310f10fU, 0x342722382a30e802U, 0xfad8f1390a0a25e4U,
  0x11b070f382209e4U, 0xa18e43711df1806U, 0xe216e33cda163ef0U, 0x13eee14a040b2410U,
  0x36e922310df41aefU, 0x2718006036290e04U, 0x3314ef2e05fd23ecU, 0xf60eda5713f52e12U,
  0x2d14014b201beb12U, 0x2717f63c16224cf2U, 0x2d06fd42fbfe0d27U, 0x2414f9491d1f09ecU,
  0xd5c0e9490bd31716U, 0x2112f417e7db1f13U, 0xbe9e3f82ef4eddbU, 0xf6ebf8ee02d60603U,
  0xea1120291eff1af7U, 0xfcf2103735dd162dU, 0x26f40b4f23fd33f7U, 0x1203ff37f92426edU,
  0xeed2de4443baf7efU, 0xdec5f4763f10e0e4U, 0x1af6e70c42e233e3U, 0x15efef55200e52e8U,
  0xe225f447f5fa3d0cU, 0xdffd10430c261d16U, 0x2db212c1de20714U, 0x14e50b5b55faeae5U,
  0x6e4c62a251b1f05U, 0xf9c2c84416cbd3f0U, 0xc8e347e4e50cc7U, 0xf0bf095434e3bf9aU,
  0xf7d7d1750dff36c8U, 0x3006d4fef7f7fbceU, 0xf508f055490415feU, 0x6f7272d382611c5U,
  0xebd2dc54e2d1fb0dU, 0xd63e49b4ee09f84cU, 0xe60212a2fc05f33eU, 0xeb343083d710a95fU,
  0xe32336cbddec0a48U, 0xe93442d2ea23dc31U, 0x72150e3ab12f04eU, 0x110e37daf0180754U,
  0xe94a39ef00ecf618U, 0xd34d23f3b804c917U, 0xe64133c4d5ddde4bU, 0xc92e27a3ac2dea20U,
  0xde3d16b191f0d331U, 0xd7110cc5bffdba22U, 0xde1d13a6ba14054dU, 0xda210cd5c709b423U,
  0x15171bb202d9c63bU, 0xdf1013fada14de47U, 0xbb321bcaf028b711U, 0xc9e20cbcdfe40015U,
  0xd523f906f42add23U, 0xe7e0fbeec70eb625U, 0xdd0310d71033b6f5U, 0xf414f3bc2935170dU,
  0xf7f22ad822fecbeaU, 0xfcfb15f608049d00U, 0xcd1bdeff16f7e4f3U, 0xe0281dbaccfaccedU,
  0xdc1a22a1d1fecb09U, 0xe1fa1ba2f815d724U, 0xcc1412b8c018e038U, 0xe5342ba6c4e2d812U,
  0xf54609d00822c41cU, 0xee2f18c1db270435U, 0xff3b02eafd20d309U, 0xaf1b14af0019ef28U,
  0xcd0d21e9e1f9b706U, 0xea1a1ebef813000fU, 0xe316f0e614f6b7ebU, 0xfdf2f8bdfb14f932U,
  0xfceeefeff0d9bd26U, 0xf1fb019adccfff1bU, 0x110801d0ca30b33cU, 0xd51550d8dc03b90aU,
  0xbd4afabacfedd443U, 0xd51c2ca0db23f306U, 0x4ff1781b911f843U, 0xeb0b0dd8b40ccc32U,
  0xea102d95eb1be534U, 0xd0251ec5ba0eea2aU, 0xf24022cdb633ae20U, 0xeb47448aab35db67U,
  0xe91228e2bd42ae45U, 0xd52b4383c9ffe135U, 0xe04c1194ec10bd5dU, 0xe32637c2e82cde3aU,
  0xec333db5e42fdc46U, 0x1220369de6fee832U, 0xe8331cdfbb32fa5eU, 0x2d1c4cd7ef36e53dU,
  0xfd3c5badb2f9d724U, 0x142a2896be10f55cU, 0x69000002eeU, 0x1186000001cfU,
  0xe87f3b451eafaa81U, 0xfffffc5500000615U,
};


ai_handle g_network_weights_table[1 + 2] =
{
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

