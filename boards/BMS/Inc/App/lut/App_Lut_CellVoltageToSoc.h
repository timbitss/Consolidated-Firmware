#pragma once

#define V_TO_SOC_LUT_SIZE (121U)

// Use the voltage to SoC lookup table to get the index. Use this index to look up the state of charge

static const float v_to_soc_lookup[V_TO_SOC_LUT_SIZE] = {
    4.17231f, 4.14838f, 4.13835f, 4.1293f,  4.12121f, 4.11312f, 4.10504f, 4.09728f, 4.08919f, 4.08175f, 4.07367f,
    4.06655f, 4.05943f, 4.05361f, 4.04908f, 4.04456f, 4.03971f, 4.03324f, 4.02353f, 4.0093f,  3.99442f, 3.98116f,
    3.96984f, 3.96079f, 3.9527f,  3.94591f, 3.94009f, 3.93459f, 3.93006f, 3.92521f, 3.921f,   3.91648f, 3.91162f,
    3.9071f,  3.90225f, 3.89707f, 3.8919f,  3.88607f, 3.88025f, 3.87508f, 3.86958f, 3.8644f,  3.85923f, 3.85405f,
    3.8492f,  3.84435f, 3.8395f,  3.83497f, 3.83044f, 3.82591f, 3.82171f, 3.81718f, 3.8133f,  3.80942f, 3.80586f,
    3.80198f, 3.79842f, 3.79486f, 3.79131f, 3.78775f, 3.78484f, 3.78128f, 3.77837f, 3.77514f, 3.77222f, 3.76931f,
    3.7664f,  3.76349f, 3.76058f, 3.75832f, 3.75541f, 3.75314f, 3.75088f, 3.74829f, 3.74603f, 3.74376f, 3.74182f,
    3.73988f, 3.73794f, 3.736f,   3.73438f, 3.73277f, 3.73147f, 3.72985f, 3.72824f, 3.72662f, 3.72533f, 3.72339f,
    3.72177f, 3.71983f, 3.71756f, 3.7153f,  3.71304f, 3.7098f,  3.70689f, 3.70398f, 3.70075f, 3.69751f, 3.69395f,
    3.69039f, 3.68651f, 3.68166f, 3.67681f, 3.67228f, 3.66905f, 3.66646f, 3.66452f, 3.66193f, 3.65935f, 3.65611f,
    3.65191f, 3.64576f, 3.63606f, 3.62086f, 3.59886f, 3.56911f, 3.53029f, 3.47919f, 3.40965f, 3.30648f, 3.07522f
};

static const float energy_lookup[V_TO_SOC_LUT_SIZE] = { 13720.845657204027f,
                                                        13624.421069240241f,
                                                        13523.644904952154f,
                                                        13420.350505257114f,
                                                        13315.271950993929f,
                                                        13208.85654120942f,
                                                        13101.414582224652f,
                                                        12993.200769201101f,
                                                        12884.439695112791f,
                                                        12775.302881911994f,
                                                        12665.93463485692f,
                                                        12556.42065499561f,
                                                        12446.790646988002f,
                                                        12337.069795362162f,
                                                        12227.281270693164f,
                                                        12117.422454651956f,
                                                        12007.497490910553f,
                                                        11897.481920858443f,
                                                        11787.347888122156f,
                                                        11677.076749704955f,
                                                        11566.657657978521f,
                                                        11456.111255154017f,
                                                        11345.446538332228f,
                                                        11234.68206219582f,
                                                        11123.815422657612f,
                                                        11012.843872789243f,
                                                        10901.782914617272f,
                                                        10790.632459408478f,
                                                        10679.386699176253f,
                                                        10568.064295828513f,
                                                        10456.676340353271f,
                                                        10345.226602479916f,
                                                        10233.708626730535f,
                                                        10122.128459008169f,
                                                        10010.499709934968f,
                                                        9898.81578267087f,
                                                        9787.084549579446f,
                                                        9675.31517834008f,
                                                        9563.493264053168f,
                                                        9451.641011902779f,
                                                        9339.733517901137f,
                                                        9227.781391601542f,
                                                        9115.785080841735f,
                                                        9003.72673912573f,
                                                        8891.602083886739f,
                                                        8779.432355164521f,
                                                        8667.18138536736f,
                                                        8554.866714672231f,
                                                        8442.484948194233f,
                                                        8330.037861029432f,
                                                        8217.516612196468f,
                                                        8104.917776567875f,
                                                        7992.240539220955f,
                                                        7879.48524803051f,
                                                        7766.650633344165f,
                                                        7653.71795475555f,
                                                        7540.707968823251f,
                                                        7427.603797351045f,
                                                        7314.411666309744f,
                                                        7201.124410385992f,
                                                        7087.741853117262f,
                                                        6974.262192167563f,
                                                        6860.685452308446f,
                                                        6747.013609502025f,
                                                        6633.229470065505f,
                                                        6519.335511550316f,
                                                        6405.331869366112f,
                                                        6291.21936464344f,
                                                        6176.978965219392f,
                                                        6062.616401835729f,
                                                        5948.133127583529f,
                                                        5833.524172487262f,
                                                        5718.791714194415f,
                                                        5603.923860040818f,
                                                        5488.921200665055f,
                                                        5373.769691622811f,
                                                        5258.480027404454f,
                                                        5143.052979284254f,
                                                        5027.461560903247f,
                                                        4911.721012931328f,
                                                        4795.820342412369f,
                                                        4679.758236846925f,
                                                        4563.535014816213f,
                                                        4447.135995866945f,
                                                        4330.577970221645f,
                                                        4213.852000615849f,
                                                        4096.977512804311f,
                                                        3979.9483487661278f,
                                                        3862.7630463619885f,
                                                        3745.4425646978957f,
                                                        3627.9981123533175f,
                                                        3510.4227950837426f,
                                                        3392.7055412504274f,
                                                        3274.850305699177f,
                                                        3156.843526262401f,
                                                        3038.666608372987f,
                                                        2920.3003079399773f,
                                                        2801.7152687411017f,
                                                        2682.8758539187825f,
                                                        2563.7249620060984f,
                                                        2444.2069948191725f,
                                                        2324.271816237988f,
                                                        2203.8775280554332f,
                                                        2083.1233655737055f,
                                                        1962.1381420170069f,
                                                        1840.9905273484133f,
                                                        1719.6996638632143f,
                                                        1598.273665614497f,
                                                        1476.6963823726671f,
                                                        1354.9265610414582f,
                                                        1232.9288182421374f,
                                                        1110.7120051320499f,
                                                        988.2599459167892f,
                                                        865.5737307632728f,
                                                        742.6536871662646f,
                                                        619.4956638275314f,
                                                        496.1018069767493f,
                                                        372.4582134133773f,
                                                        248.5572662408067f,
                                                        124.37066316049665f,
                                                        0.0f };
