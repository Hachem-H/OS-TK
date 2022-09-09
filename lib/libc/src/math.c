#include "math.h"
#include "errno.h"

#pragma GCC diagnostic ignored "-Wdiv-by-zero"

#define HIGH(x) *(1 + (int *)&x)
#define HIGH_POINTER(x) *(1 + (int *)x)
#define LOW(x) *(int *)&x
#define LOW_POINTER(x) *(int *)x

static const double one = 1.0;
static const double two = 2.0;
static const double zero[] = {0.0, -0.0};

static const double Lg1 = 6.666666666666735130e-01;
static const double Lg2 = 3.999999999940941908e-01;
static const double Lg3 = 2.857142874366239149e-01;
static const double Lg4 = 2.222219843214978396e-01;
static const double Lg5 = 1.818357216161805012e-01;
static const double Lg6 = 1.531383769920937332e-01;
static const double Lg7 = 1.479819860511658591e-01;

static const double halF[2] = {
    0.5,
    -0.5,
};
static const double twom1000 = 9.33263618503218878990e-302;
static const double twom54 = 5.55111512312578270212e-17;
static const double o_threshold = 7.09782712893383973096e+02;
static const double u_threshold = -7.45133219101941108420e+02;

static const double ln2HI[2] = {
    6.93147180369123816490e-01,
    -6.93147180369123816490e-01,
};
static const double ln2LO[2] = {
    1.90821492927058770002e-10,
    -1.90821492927058770002e-10,
};

static const double bp[] = {
    1.0,
    1.5,
};
static const double dp_h[] = {
    0.0,
    5.84962487220764160156e-01,
};
static const double dp_l[] = {
    0.0,
    1.35003920212974897128e-08,
};

static const double half = 0.5;
static const double huge = 1.0e300;
static const double shuge = 1.0e307;
static const double tiny = 1.0e-300;

static const double two53 = 9007199254740992.0;
static const double two54 = 1.80143985094819840000e+16;

static const double L1 = 5.99999999999994648725e-01;
static const double L2 = 4.28571428578550184252e-01;
static const double L3 = 3.33333329818377432918e-01;
static const double L4 = 2.72728123808534006489e-01;
static const double L5 = 2.30660745775561754067e-01;
static const double L6 = 2.06975017800338417784e-01;

static const double P1 = 1.66666666666666019037e-01;
static const double P2 = -2.77777777770155933842e-03;
static const double P3 = 6.61375632143793436117e-05;
static const double P4 = -1.65339022054652515390e-06;
static const double P5 = 4.13813679705723846039e-08;

static const double lg2 = 6.93147180559945286227e-01;
static const double lg2_h = 6.93147182464599609375e-01;
static const double lg2_l = -1.90465429995776804525e-09;

static const double ovt = 8.0085662595372944372e-0017;
static const double cp = 9.61796693925975554329e-01;
static const double cp_h = 9.61796700954437255859e-01;
static const double cp_l = -7.02846165095275826516e-09;

static const double invln2 = 1.44269504088896338700e+00;
static const double ln2_hi = 6.93147180369123816490e-01;
static const double ln2_lo = 1.90821492927058770002e-10;
static const double ivln2 = 1.44269504088896338700e+00;
static const double ivln10 = 4.34294481903251816668e-01;
static const double ivln2_h = 1.44269502162933349609e+00;
static const double ivln2_l = 1.92596299112661746887e-08;

static const double pi = 3.1415926535897931160E+00;
static const double pi_lo = 1.2246467991473531772E-16;
static const double pio2_hi = 1.57079632679489655800e+00;
static const double pio2_lo = 6.12323399573676603587e-17;
static const double pio4_hi = 7.85398163397448278999e-01;
static const double pi_o_4 = 7.8539816339744827900E-01;
static const double pi_o_2 = 1.5707963267948965580E+00;

static const double pS0 = 1.66666666666666657415e-01;
static const double pS1 = -3.25565818622400915405e-01;
static const double pS2 = 2.01212532134862925881e-01;
static const double pS3 = -4.00555345006794114027e-02;
static const double pS4 = 7.91534994289814532176e-04;
static const double pS5 = 3.47933107596021167570e-05;

static const double qS1 = -2.40339491173441421878e+00;
static const double qS2 = 2.02094576023350569471e+00;
static const double qS3 = -6.88283971605453293030e-01;
static const double qS4 = 7.70381505559019352791e-02;

static const double Q1 = -3.33333333333331316428e-02;
static const double Q2 = 1.58730158725481460165e-03;
static const double Q3 = -7.93650757867487942473e-05;
static const double Q4 = 4.00821782732936239552e-06;
static const double Q5 = -2.01099218183624371326e-07;

static const double log10_2hi = 3.01029995663611771306e-01;
static const double log10_2lo = 3.69423907715893078616e-13;

static unsigned int finite(double x)
{
    return (unsigned int)((HIGH(x) & 0x7fffffff) - 0x7ff00000) >> 31;
}

static double copysign(double x, double y)
{
    HIGH(x) = (HIGH(x) & 0x7fffffff) | (HIGH(y) & 0x80000000);
    return x;
}

static double scalbn(double x, int n)
{
    int k, hx, lx;
    hx = HIGH(x);
    lx = LOW(x);
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0)
    {
        if ((lx | (hx & 0x7fffffff)) == 0)
            return x;
        x *= two54;
        hx = HIGH(x);
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000)
            return tiny * x;
    }
    if (k == 0x7ff)
        return x + x;
    k = k + n;
    if (k > 0x7fe)
        return huge * copysign(huge, x);
    if (k > 0)
    {
        HIGH(x) = (hx & 0x800fffff) | (k << 20);
        return x;
    }
    if (k <= -54)
    {
        if (n > 50000)
            return huge * copysign(huge, x);
        else
            return tiny * copysign(tiny, x);
    }
    k += 54;
    HIGH(x) = (hx & 0x800fffff) | (k << 20);
    return x * twom54;
}

static double expm1(double x)
{
    double y, hi, lo, c, t, e, hxs, hfx, r1;
    int k, xsb;
    unsigned hx;

    hx = HIGH(x);
    xsb = hx & 0x80000000;
    if (xsb == 0)
        y = x;
    else
        y = -x;
    hx &= 0x7fffffff;

    if (hx >= 0x4043687A)
    {
        if (hx >= 0x40862E42)
        {
            if (hx >= 0x7ff00000)
            {
                if (((hx & 0xfffff) | LOW(x)) != 0)
                    return x + x;
                else
                    return (xsb == 0) ? x : -1.0;
            }
            if (x > o_threshold)
                return huge * huge;
        }
        if (xsb != 0)
        {
            if (x + tiny < 0.0)
                return tiny - one;
        }
    }

    if (hx > 0x3fd62e42)
    {
        if (hx < 0x3FF0A2B2)
        {
            if (xsb == 0)
            {
                hi = x - ln2_hi;
                lo = ln2_lo;
                k = 1;
            }
            else
            {
                hi = x + ln2_hi;
                lo = -ln2_lo;
                k = -1;
            }
        }
        else
        {
            k = invln2 * x + ((xsb == 0) ? 0.5 : -0.5);
            t = k;
            hi = x - t * ln2_hi;
            lo = t * ln2_lo;
        }
        x = hi - lo;
        c = (hi - x) - lo;
    }
    else if (hx < 0x3c900000)
    {
        t = huge + x;
        return x - (t - (huge + x));
    }
    else
        k = 0;

    hfx = 0.5 * x;
    hxs = x * hfx;
    r1 = one + hxs * (Q1 + hxs * (Q2 + hxs * (Q3 + hxs * (Q4 + hxs * Q5))));
    t = 3.0 - r1 * hfx;
    e = hxs * ((r1 - t) / (6.0 - x * t));
    if (k == 0)
        return x - (x * e - hxs);
    else
    {
        e = (x * (e - c) - c);
        e -= hxs;
        if (k == -1)
            return 0.5 * (x - e) - 0.5;
        if (k == 1)
        {
            if (x < -0.25)
                return -2.0 * (e - (x + 0.5));
            else
                return one + 2.0 * (x - e);
        }
        if (k <= -2 || k > 56)
        {
            y = one - (e - x);
            HIGH(y) += (k << 20);
            return y - one;
        }
        t = one;
        if (k < 20)
        {
            HIGH(t) = 0x3ff00000 - (0x200000 >> k);
            y = t - (e - x);
            HIGH(y) += (k << 20);
        }
        else
        {
            HIGH(t) = ((0x3ff - k) << 20);
            y = x - (e + t);
            y += one;
            HIGH(y) += (k << 20);
        }
    }
    return y;
}

double sin(double x)
{
    double t, w, p, q, c, r, s;
    int hx, ix;
    hx = HIGH(x);
    ix = hx & 0x7fffffff;
    if (ix >= 0x3ff00000)
    {
        if (((ix - 0x3ff00000) | LOW(x)) == 0)

            return x * pio2_hi + x * pio2_lo;
        return (x - x) / (x - x);
    }
    else if (ix < 0x3fe00000)
    {
        if (ix < 0x3e400000)
        {
            if (huge + x > one)
                return x;
        }
        else
            t = x * x;
        p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
        q = one + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
        w = p / q;
        return x + x * w;
    }

    w = one - fabs(x);
    t = w * 0.5;
    p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
    q = one + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
    s = sqrt(t);
    if (ix >= 0x3FEF3333)
    {
        w = p / q;
        t = pio2_hi - (2.0 * (s + s * w) - pio2_lo);
    }
    else
    {
        w = s;
        LOW(w) = 0;
        c = (t - w * w) / (s + w);
        r = p / q;
        p = 2.0 * s * r - (pio2_lo - 2.0 * c);
        q = pio4_hi - 2.0 * w;
        t = pio4_hi - (p - q);
    }
    if (hx > 0)
        return t;
    else
        return -t;
}

double cos(double x)
{
    double z, p, q, r, w, s, c, df;
    int hx, ix;
    hx = HIGH(x);
    ix = hx & 0x7fffffff;
    if (ix >= 0x3ff00000)
    {
        if (((ix - 0x3ff00000) | LOW(x)) == 0)
        {
            if (hx > 0)
                return 0.0;
            else
                return pi + 2.0 * pio2_lo;
        }
        return (x - x) / (x - x);
    }
    if (ix < 0x3fe00000)
    {
        if (ix <= 0x3c600000)
            return pio2_hi + pio2_lo;
        z = x * x;
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        r = p / q;
        return pio2_hi - (x - (pio2_lo - x * r));
    }
    else if (hx < 0)
    {
        z = (one + x) * 0.5;
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        s = sqrt(z);
        r = p / q;
        w = r * s - pio2_lo;
        return pi - 2.0 * (s + w);
    }
    else
    {
        z = (one - x) * 0.5;
        s = sqrt(z);
        df = s;
        LOW(df) = 0;
        c = (z - df * df) / (s + df);
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        r = p / q;
        w = r * s + c;
        return 2.0 * (df + w);
    }
}

double tan(double x)
{
    return sin(x) / cos(x);
}

double asin(double x)
{
    double t, w, p, q, c, r, s;
    int hx, ix;
    hx = HIGH(x);
    ix = hx & 0x7fffffff;
    if (ix >= 0x3ff00000)
    {
        if (((ix - 0x3ff00000) | LOW(x)) == 0)

            return x * pio2_hi + x * pio2_lo;
        return (x - x) / (x - x);
    }
    else if (ix < 0x3fe00000)
    {
        if (ix < 0x3e400000)
        {
            if (huge + x > one)
                return x;
        }
        else
            t = x * x;
        p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
        q = one + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
        w = p / q;
        return x + x * w;
    }

    w = one - fabs(x);
    t = w * 0.5;
    p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
    q = one + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
    s = sqrt(t);
    if (ix >= 0x3FEF3333)
    {
        w = p / q;
        t = pio2_hi - (2.0 * (s + s * w) - pio2_lo);
    }
    else
    {
        w = s;
        LOW(w) = 0;
        c = (t - w * w) / (s + w);
        r = p / q;
        p = 2.0 * s * r - (pio2_lo - 2.0 * c);
        q = pio4_hi - 2.0 * w;
        t = pio4_hi - (p - q);
    }
    if (hx > 0)
        return t;
    else
        return -t;
}

double acos(double x)
{
    double z, p, q, r, w, s, c, df;
    int hx, ix;
    hx = HIGH(x);
    ix = hx & 0x7fffffff;
    if (ix >= 0x3ff00000)
    {
        if (((ix - 0x3ff00000) | LOW(x)) == 0)
        {
            if (hx > 0)
                return 0.0;
            else
                return pi + 2.0 * pio2_lo;
        }
        return (x - x) / (x - x);
    }
    if (ix < 0x3fe00000)
    {
        if (ix <= 0x3c600000)
            return pio2_hi + pio2_lo;
        z = x * x;
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        r = p / q;
        return pio2_hi - (x - (pio2_lo - x * r));
    }
    else if (hx < 0)
    {
        z = (one + x) * 0.5;
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        s = sqrt(z);
        r = p / q;
        w = r * s - pio2_lo;
        return pi - 2.0 * (s + w);
    }
    else
    {
        z = (one - x) * 0.5;
        s = sqrt(z);
        df = s;
        LOW(df) = 0;
        c = (z - df * df) / (s + df);
        p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
        q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
        r = p / q;
        w = r * s + c;
        return 2.0 * (df + w);
    }
}

double atan(double x)
{
    return cos(x) / sin(x);
}

double atan2(double y, double x)
{
    double z;
    int k, m, hx, hy, ix, iy;
    unsigned lx, ly;

    hx = HIGH(x);
    ix = hx & 0x7fffffff;
    lx = LOW(x);
    hy = HIGH(y);
    iy = hy & 0x7fffffff;
    ly = LOW(y);
    if (((ix | ((lx | -lx) >> 31)) > 0x7ff00000) ||
        ((iy | ((ly | -ly) >> 31)) > 0x7ff00000))
        return x + y;
    if (((hx - 0x3ff00000) | lx) == 0)
        return atan(y);
    m = ((hy >> 31) & 1) | ((hx >> 30) & 2);

    if ((iy | ly) == 0)
    {
        switch (m)
        {
        case 0:
        case 1:
            return y;
        case 2:
            return pi + tiny;
        case 3:
            return -pi - tiny;
        }
    }

    if ((ix | lx) == 0)
        return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

    if (ix == 0x7ff00000)
    {
        if (iy == 0x7ff00000)
        {
            switch (m)
            {
            case 0:
                return pi_o_4 + tiny;
            case 1:
                return -pi_o_4 - tiny;
            case 2:
                return 3.0 * pi_o_4 + tiny;
            case 3:
                return -3.0 * pi_o_4 - tiny;
            }
        }
        else
        {
            switch (m)
            {
            case 0:
                return 0;
            case 1:
                return -0;
            case 2:
                return pi + tiny;
            case 3:
                return -pi - tiny;
            }
        }
    }

    if (iy == 0x7ff00000)
        return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

    k = (iy - ix) >> 20;
    if (k > 60)
        z = pi_o_2 + 0.5 * pi_lo;
    else if (hx < 0 && k < -60)
        z = 0.0;
    else
        z = atan(fabs(y / x));
    switch (m)
    {
    case 0:
        return z;
    case 1:
        HIGH(z) ^= 0x80000000;
        return z;
    case 2:
        return pi - (z - pi_lo);
    default:
        return (z - pi_lo) - pi;
    }
}

double sinh(double x)
{
    double t, w, h;
    int ix, jx;
    unsigned lx;

    jx = HIGH(x);
    ix = jx & 0x7fffffff;

    if (ix >= 0x7ff00000)
        return x + x;

    h = 0.5;
    if (jx < 0)
        h = -h;

    if (ix < 0x40360000)
    {
        if (ix < 0x3e300000)
            if (shuge + x > one)
                return x;
        t = expm1(fabs(x));
        if (ix < 0x3ff00000)
            return h * (2.0 * t - t * t / (t + one));
        return h * (t + t / (t + one));
    }

    if (ix < 0x40862E42)
        return h * exp(fabs(x));

    lx = *((((*(unsigned *)&one) >> 29)) + (unsigned *)&x);
    if (ix < 0x408633CE || ((ix == 0x408633ce) && (lx <= (unsigned)0x8fb9f87d)))
    {
        w = exp(0.5 * fabs(x));
        t = h * w;
        return t * w;
    }

    return x * shuge;
}

double cosh(double x)
{
    double t, w;
    int ix;
    unsigned lx;

    ix = HIGH(x);
    ix &= 0x7fffffff;

    if (ix >= 0x7ff00000)
        return x * x;

    if (ix < 0x3fd62e43)
    {
        t = expm1(fabs(x));
        w = one + t;
        if (ix < 0x3c800000)
            return w;
        return one + (t * t) / (w + w);
    }

    if (ix < 0x40360000)
    {
        t = exp(fabs(x));
        return half * t + half / t;
    }

    if (ix < 0x40862E42)
        return half * exp(fabs(x));

    lx = *((((*(unsigned *)&one) >> 29)) + (unsigned *)&x);
    if (ix < 0x408633CE ||
        ((ix == 0x408633ce) && (lx <= (unsigned)0x8fb9f87d)))
    {
        w = exp(half * fabs(x));
        t = half * w;
        return t * w;
    }

    return huge * huge;
}

double tanh(double x)
{
    return sinh(x) - cosh(x);
}

double exp(double x)
{
    double y, hi, lo, c, t;
    int k, xsb;
    unsigned hx;

    hx = HIGH(x);
    xsb = (hx >> 31) & 1;
    hx &= 0x7fffffff;

    if (hx >= 0x40862E42)
    {
        if (hx >= 0x7ff00000)
        {
            if (((hx & 0xfffff) | LOW(x)) != 0)
                return x + x;
            else
                return (xsb == 0) ? x : 0.0;
        }
        if (x > o_threshold)
            return huge * huge;
        if (x < u_threshold)
            return twom1000 * twom1000;
    }

    if (hx > 0x3fd62e42)
    {
        if (hx < 0x3FF0A2B2)
        {
            hi = x - ln2HI[xsb];
            lo = ln2LO[xsb];
            k = 1 - xsb - xsb;
        }
        else
        {
            k = (int)(invln2 * x + halF[xsb]);
            t = k;
            hi = x - t * ln2HI[0];
            lo = t * ln2LO[0];
        }
        x = hi - lo;
    }
    else if (hx < 0x3e300000)
    {
        if (huge + x > one)
            return one + x;
    }
    else
        k = 0;

    t = x * x;
    c = x - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
    if (k == 0)
        return one - ((x * c) / (c - 2.0) - x);
    else
        y = one - ((lo - (x * c) / (2.0 - c)) - hi);
    if (k >= -1021)
    {
        HIGH(y) += (k << 20);
        return y;
    }
    else
    {
        HIGH(y) += ((k + 1000) << 20);
        return y * twom1000;
    }
}

double log(double x)
{
    double hfsq, f, s, z, R, w, t1, t2, dk;
    int k, hx, i, j;
    unsigned lx;

    hx = HIGH(x);
    lx = LOW(x);

    k = 0;
    if (hx < 0x00100000)
    {
        if (((hx & 0x7fffffff) | lx) == 0)
            return -two54 / 0;
        if (hx < 0)
            return (x - x) / 0;
        k -= 54;
        x *= two54;
        hx = HIGH(x);
    }
    if (hx >= 0x7ff00000)
        return x + x;
    k += (hx >> 20) - 1023;
    hx &= 0x000fffff;
    i = (hx + 0x95f64) & 0x100000;
    HIGH(x) = hx | (i ^ 0x3ff00000);
    k += (i >> 20);
    f = x - 1.0;
    if ((0x000fffff & (2 + hx)) < 3)
    {
        if (f == 0)
        {
            if (k == 0)
                return 0;
            else
            {
                dk = (double)k;
                return dk * ln2_hi + dk * ln2_lo;
            }
        }
        R = f * f * (0.5 - 0.33333333333333333 * f);
        if (k == 0)
            return f - R;
        else
        {
            dk = (double)k;
            return dk * ln2_hi - ((R - dk * ln2_lo) - f);
        }
    }
    s = f / (2.0 + f);
    dk = (double)k;
    z = s * s;
    i = hx - 0x6147a;
    w = z * z;
    j = 0x6b851 - hx;
    t1 = w * (Lg2 + w * (Lg4 + w * Lg6));
    t2 = z * (Lg1 + w * (Lg3 + w * (Lg5 + w * Lg7)));
    i |= j;
    R = t2 + t1;
    if (i > 0)
    {
        hfsq = 0.5 * f * f;
        if (k == 0)
            return f - (hfsq - s * (hfsq + R));
        else
            return dk * ln2_hi - ((hfsq - (s * (hfsq + R) + dk * ln2_lo)) - f);
    }
    else
    {
        if (k == 0)
            return f - s * (f - R);
        else
            return dk * ln2_hi - ((s * (f - R) - dk * ln2_lo) - f);
    }
}

double log10(double x)
{
    double y, z;
    int i, k, hx;
    unsigned lx;

    hx = HIGH(x);
    lx = LOW(x);

    k = 0;
    if (hx < 0x00100000)
    {
        if (((hx & 0x7fffffff) | lx) == 0)
            return -two54 / 0;
        if (hx < 0)
            return (x - x) / 0;
        k -= 54;
        x *= two54;
        hx = HIGH(x);
    }
    if (hx >= 0x7ff00000)
        return x + x;
    k += (hx >> 20) - 1023;
    i = ((unsigned)k & 0x80000000) >> 31;
    hx = (hx & 0x000fffff) | ((0x3ff - i) << 20);
    y = (double)(k + i);
    HIGH(x) = hx;
    z = y * log10_2lo + ivln10 * log(x);
    return z + y * log10_2hi;
}

double pow(double x, double y)
{
    double z, ax, z_h, z_l, p_h, p_l;
    double y1, t1, t2, r, s, t, u, v, w;
    int i, j, k, yisint, n;
    int hx, hy, ix, iy;
    unsigned lx, ly;

    hx = HIGH(x);
    lx = LOW(x);
    hy = HIGH(y);
    ly = LOW(y);
    ix = hx & 0x7fffffff;
    iy = hy & 0x7fffffff;

    if ((iy | ly) == 0)
        return one;

    if (ix > 0x7ff00000 || ((ix == 0x7ff00000) && (lx != 0)) ||
        iy > 0x7ff00000 || ((iy == 0x7ff00000) && (ly != 0)))
        return x + y;

    yisint = 0;
    if (hx < 0)
    {
        if (iy >= 0x43400000)
            yisint = 2;
        else if (iy >= 0x3ff00000)
        {
            k = (iy >> 20) - 0x3ff;
            if (k > 20)
            {
                j = ly >> (52 - k);
                if ((unsigned int)(j << (52 - k)) == ly)
                    yisint = 2 - (j & 1);
            }
            else if (ly == 0)
            {
                j = iy >> (20 - k);
                if ((j << (20 - k)) == iy)
                    yisint = 2 - (j & 1);
            }
        }
    }

    if (ly == 0)
    {
        if (iy == 0x7ff00000)
        {
            if (((ix - 0x3ff00000) | lx) == 0)
                return y - y;
            else if (ix >= 0x3ff00000)
                return (hy >= 0) ? y : 0;
            else
                return (hy < 0) ? -y : 0;
        }
        if (iy == 0x3ff00000)
        {
            if (hy < 0)
                return one / x;
            else
                return x;
        }
        if (hy == 0x40000000)
            return x * x;
        if (hy == 0x3fe00000)
        {
            if (hx >= 0)
                return sqrt(x);
        }
    }

    ax = fabs(x);

    if (lx == 0)
    {
        if (ix == 0x7ff00000 || ix == 0 || ix == 0x3ff00000)
        {
            z = ax;
            if (hy < 0)
                z = one / z;
            if (hx < 0)
            {
                if (((ix - 0x3ff00000) | yisint) == 0)
                {
                    z = (z - z) / (z - z);
                }
                else if (yisint == 1)
                    z = -z;
            }
            return z;
        }
    }

    n = (hx >> 31) + 1;

    if ((n | yisint) == 0)
        return (x - x) / (x - x);

    s = one;
    if ((n | (yisint - 1)) == 0)
        s = -one;

    if (iy > 0x41e00000)
    {
        if (iy > 0x43f00000)
        {
            if (ix <= 0x3fefffff)
                return (hy < 0) ? huge * huge : tiny * tiny;
            if (ix >= 0x3ff00000)
                return (hy > 0) ? huge * huge : tiny * tiny;
        }

        if (ix < 0x3fefffff)
            return (hy < 0) ? s * huge * huge : s * tiny * tiny;
        if (ix > 0x3ff00000)
            return (hy > 0) ? s * huge * huge : s * tiny * tiny;

        t = ax - one;
        w = (t * t) * (0.5 - t * (0.3333333333333333333333 - t * 0.25));
        u = ivln2_h * t;
        v = t * ivln2_l - w * ivln2;
        t1 = u + v;
        LOW(t1) = 0;
        t2 = v - (t1 - u);
    }
    else
    {
        double ss, s2, s_h, s_l, t_h, t_l;
        n = 0;

        if (ix < 0x00100000)
        {
            ax *= two53;
            n -= 53;
            ix = HIGH(ax);
        }
        n += ((ix) >> 20) - 0x3ff;
        j = ix & 0x000fffff;

        ix = j | 0x3ff00000;
        if (j <= 0x3988E)
            k = 0;
        else if (j < 0xBB67A)
            k = 1;
        else
        {
            k = 0;
            n += 1;
            ix -= 0x00100000;
        }
        HIGH(ax) = ix;

        u = ax - bp[k];
        v = one / (ax + bp[k]);
        ss = u * v;
        s_h = ss;
        LOW(s_h) = 0;

        t_h = 0;
        HIGH(t_h) = ((ix >> 1) | 0x20000000) + 0x00080000 + (k << 18);
        t_l = ax - (t_h - bp[k]);
        s_l = v * ((u - s_h * t_h) - s_h * t_l);

        s2 = ss * ss;
        r = s2 * s2 * (L1 + s2 * (L2 + s2 * (L3 + s2 * (L4 + s2 * (L5 + s2 * L6)))));
        r += s_l * (s_h + ss);
        s2 = s_h * s_h;
        t_h = 3.0 + s2 + r;
        LOW(t_h) = 0;
        t_l = r - ((t_h - 3.0) - s2);

        u = s_h * t_h;
        v = s_l * t_h + t_l * ss;

        p_h = u + v;
        LOW(p_h) = 0;
        p_l = v - (p_h - u);
        z_h = cp_h * p_h;
        z_l = cp_l * p_h + p_l * cp + dp_l[k];

        t = (double)n;
        t1 = (((z_h + z_l) + dp_h[k]) + t);
        LOW(t1) = 0;
        t2 = z_l - (((t1 - t) - dp_h[k]) - z_h);
    }

    y1 = y;
    LOW(y1) = 0;
    p_l = (y - y1) * t1 + y * t2;
    p_h = y1 * t1;
    z = p_l + p_h;
    j = HIGH(z);
    i = LOW(z);
    if (j >= 0x40900000)
    {
        if (((j - 0x40900000) | i) != 0)
            return s * huge * huge;
        else
        {
            if (p_l + ovt > z - p_h)
                return s * huge * huge;
        }
    }
    else if ((j & 0x7fffffff) >= 0x4090cc00)
    {
        if (((j - 0xc090cc00) | i) != 0)
            return s * tiny * tiny;
        else
        {
            if (p_l <= z - p_h)
                return s * tiny * tiny;
        }
    }

    i = j & 0x7fffffff;
    k = (i >> 20) - 0x3ff;
    n = 0;
    if (i > 0x3fe00000)
    {
        n = j + (0x00100000 >> (k + 1));
        k = ((n & 0x7fffffff) >> 20) - 0x3ff;
        t = 0;
        HIGH(t) = (n & ~(0x000fffff >> k));
        n = ((n & 0x000fffff) | 0x00100000) >> (20 - k);
        if (j < 0)
            n = -n;
        p_h -= t;
    }
    t = p_l + p_h;
    LOW(t) = 0;
    u = t * lg2_h;
    v = (p_l - (t - p_h)) * lg2 + t * lg2_l;
    z = u + v;
    w = v - (z - u);
    t = z * z;
    t1 = z - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
    r = (z * t1) / (t1 - two) - (w + z * w);
    z = one - (r - z);
    j = HIGH(z);
    j += (n << 20);
    if ((j >> 20) <= 0)
        z = scalbn(z, n);
    else
        HIGH(z) += (n << 20);
    return s * z;
}

double sqrt(double x)
{
    double z;
    int sign = (int)0x80000000;
    unsigned r, t1, s1, ix1, q1;
    int ix0, s0, q, m, t, i;

    ix0 = HIGH(x);
    ix1 = LOW(x);

    if ((ix0 & 0x7ff00000) == 0x7ff00000)
    {
        return x * x + x;
    }

    if (ix0 <= 0)
    {
        if (((ix0 & (~sign)) | ix1) == 0)
            return x;
        else if (ix0 < 0)
            return (x - x) / (x - x);
    }

    m = (ix0 >> 20);
    if (m == 0)
    {
        while (ix0 == 0)
        {
            m -= 21;
            ix0 |= (ix1 >> 11);
            ix1 <<= 21;
        }
        for (i = 0; (ix0 & 0x00100000) == 0; i++)
            ix0 <<= 1;
        m -= i - 1;
        ix0 |= (ix1 >> (32 - i));
        ix1 <<= i;
    }
    m -= 1023;
    ix0 = (ix0 & 0x000fffff) | 0x00100000;
    if (m & 1)
    {
        ix0 += ix0 + ((ix1 & sign) >> 31);
        ix1 += ix1;
    }
    m >>= 1;

    ix0 += ix0 + ((ix1 & sign) >> 31);
    ix1 += ix1;
    q = q1 = s0 = s1 = 0;
    r = 0x00200000;

    while (r != 0)
    {
        t = s0 + r;
        if (t <= ix0)
        {
            s0 = t + r;
            ix0 -= t;
            q += r;
        }
        ix0 += ix0 + ((ix1 & sign) >> 31);
        ix1 += ix1;
        r >>= 1;
    }

    r = sign;
    while (r != 0)
    {
        t1 = s1 + r;
        t = s0;
        if ((t < ix0) || ((t == ix0) && (t1 <= ix1)))
        {
            s1 = t1 + r;
            if (((t1 & sign) == (unsigned int)sign) && (s1 & sign) == 0)
                s0 += 1;
            ix0 -= t;
            if (ix1 < t1)
                ix0 -= 1;
            ix1 -= t1;
            q1 += r;
        }
        ix0 += ix0 + ((ix1 & sign) >> 31);
        ix1 += ix1;
        r >>= 1;
    }

    if ((ix0 | ix1) != 0)
    {
        z = one - tiny;
        if (z >= one)
        {
            z = one + tiny;
            if (q1 == (unsigned)0xffffffff)
            {
                q1 = 0;
                q += 1;
            }
            else if (z > one)
            {
                if (q1 == (unsigned)0xfffffffe)
                    q += 1;
                q1 += 2;
            }
            else
                q1 += (q1 & 1);
        }
    }
    ix0 = (q >> 1) + 0x3fe00000;
    ix1 = q1 >> 1;
    if ((q & 1) == 1)
        ix1 |= sign;
    ix0 += (m << 20);
    HIGH(z) = ix0;
    LOW(z) = ix1;
    return z;
}

double ceil(double x)
{
    int i0, i1, j0;
    unsigned i, j;
    i0 = HIGH(x);
    i1 = LOW(x);
    j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
    if (j0 < 20)
    {
        if (j0 < 0)
        {
            if (huge + x > 0.0)
            {
                if (i0 < 0)
                {
                    i0 = 0x80000000;
                    i1 = 0;
                }
                else if ((i0 | i1) != 0)
                {
                    i0 = 0x3ff00000;
                    i1 = 0;
                }
            }
        }
        else
        {
            i = (0x000fffff) >> j0;
            if (((i0 & i) | i1) == 0)
                return x;
            if (huge + x > 0.0)
            {
                if (i0 > 0)
                    i0 += (0x00100000) >> j0;
                i0 &= (~i);
                i1 = 0;
            }
        }
    }
    else if (j0 > 51)
    {
        if (j0 == 0x400)
            return x + x;
        else
            return x;
    }
    else
    {
        i = ((unsigned)(0xffffffff)) >> (j0 - 20);
        if ((i1 & i) == 0)
            return x;
        if (huge + x > 0.0)
        {
            if (i0 > 0)
            {
                if (j0 == 20)
                    i0 += 1;
                else
                {
                    j = i1 + (1 << (52 - j0));
                    if (j < (unsigned int)i1)
                        i0 += 1;
                    i1 = j;
                }
            }
            i1 &= (~i);
        }
    }
    HIGH(x) = i0;
    LOW(x) = i1;
    return x;
}

double floor(double x)
{
    int i0, i1, j0;
    unsigned i, j;
    i0 = HIGH(x);
    i1 = LOW(x);
    j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
    if (j0 < 20)
    {
        if (j0 < 0)
        {
            if (huge + x > 0.0)
            {
                if (i0 >= 0)
                    i0 = i1 = 0;
                else if (((i0 & 0x7fffffff) | i1) != 0)
                {
                    i0 = 0xbff00000;
                    i1 = 0;
                }
            }
        }
        else
        {
            i = (0x000fffff) >> j0;
            if (((i0 & i) | i1) == 0)
                return x;
            if (huge + x > 0.0)
            {
                if (i0 < 0)
                    i0 += (0x00100000) >> j0;
                i0 &= (~i);
                i1 = 0;
            }
        }
    }
    else if (j0 > 51)
    {
        if (j0 == 0x400)
            return x + x;
        else
            return x;
    }
    else
    {
        i = ((unsigned)(0xffffffff)) >> (j0 - 20);
        if ((i1 & i) == 0)
            return x;
        if (huge + x > 0.0)
        {
            if (i0 < 0)
            {
                if (j0 == 20)
                    i0 += 1;
                else
                {
                    j = i1 + (1 << (52 - j0));
                    if (j < (unsigned int)i1)
                        i0 += 1;
                    i1 = j;
                }
            }
            i1 &= (~i);
        }
    }

    HIGH(x) = i0;
    LOW(x) = i1;
    return x;
}

double fabs(double x)
{
    HIGH(x) &= 0x7fffffff;
    return x;
}

double ldexp(double value, int exp)
{
    if (!finite(value) || value == 0.0)
        return value;
    value = scalbn(value, exp);
    if (!finite(value) || value == 0.0)
        errno = -1;
    return value;
}

double frexp(double x, int *eptr)
{
    int hx, ix, lx;
    hx = HIGH(x);
    ix = 0x7fffffff & hx;
    lx = LOW(x);
    *eptr = 0;
    if (ix >= 0x7ff00000 || ((ix | lx) == 0))
        return x;
    if (ix < 0x00100000)
    {
        x *= two54;
        hx = HIGH(x);
        ix = hx & 0x7fffffff;
        *eptr = -54;
    }
    *eptr += (ix >> 20) - 1022;
    hx = (hx & 0x800fffff) | 0x3fe00000;
    HIGH(x) = hx;
    return x;
}

double modf(double x, double *iptr)
{
    int i0, i1, j0;
    unsigned i;

    i0 = HIGH(x);
    i1 = LOW(x);
    j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;

    if (j0 < 20)
    {
        if (j0 < 0)
        {
            HIGH_POINTER(iptr) = i0 & 0x80000000;
            LOW_POINTER(iptr) = 0;
            return x;
        }
        else
        {
            i = (0x000fffff) >> j0;
            if (((i0 & i) | i1) == 0)
            {
                *iptr = x;
                HIGH(x) &= 0x80000000;
                LOW(x) = 0;
                return x;
            }
            else
            {
                HIGH_POINTER(iptr) = i0 & (~i);
                LOW_POINTER(iptr) = 0;
                return x - *iptr;
            }
        }
    }
    else if (j0 > 51)
    {
        *iptr = x * one;
        HIGH(x) &= 0x80000000;
        LOW(x) = 0;
        return x;
    }
    else
    {
        i = ((unsigned)(0xffffffff)) >> (j0 - 20);
        if ((i1 & i) == 0)
        {
            *iptr = x;
            HIGH(x) &= 0x80000000;
            LOW(x) = 0;
            return x;
        }
        else
        {
            HIGH_POINTER(iptr) = i0;
            LOW_POINTER(iptr) = i1 & (~i);
            return x - *iptr;
        }
    }
}

double fmod(double x, double y)
{
    int hx = HIGH(x);
    int hy = HIGH(y);
    int lx = LOW(x);
    int ly = LOW(y);

    unsigned int lz = 0;

    int hz = 0;
    int sx = hx & 0x80000000;

    int i = 0;
    int n = 0;
    int ix = 0;
    int iy = 0;

    hx ^= sx;
    hy &= 0x7fffffff;

    if ((hy | ly) == 0 ||
        (hx >= 0x7ff00000) ||
        ((hy | ((ly | -ly) >> 31)) > 0x7ff00000))
        return (x * y) / (x * y);

    if (hx <= hy)
    {
        if ((hx < hy) || (lx < ly))
            return x;
        if (lx == ly)
            return zero[(unsigned)sx >> 31];
    }

    if (hx < 0x00100000)
    {
        if (hx == 0)
            for (ix = -1043, i = lx; i > 0; i <<= 1)
                ix -= 1;
        else
            for (ix = -1022, i = (hx << 11); i > 0; i <<= 1)
                ix -= 1;
    }
    else
        ix = (hx >> 20) - 1023;

    if (hy < 0x00100000)
    {
        if (hy == 0)
            for (iy = -1043, i = ly; i > 0; i <<= 1)
                iy -= 1;
        else
            for (iy = -1022, i = (hy << 11); i > 0; i <<= 1)
                iy -= 1;
    }
    else
        iy = (hy >> 20) - 1023;

    if (ix >= -1022)
        hx = 0x00100000 | (0x000fffff & hx);
    else
    {
        n = -1022 - ix;

        if (n <= 31)
        {
            hx = (hx << n) | (lx >> (32 - n));
            lx <<= n;
        }
        else
        {
            hx = lx << (n - 32);
            lx = 0;
        }
    }

    if (iy >= -1022)
        hy = 0x00100000 | (0x000fffff & hy);
    else
    {
        n = -1022 - iy;
        if (n <= 31)
        {
            hy = (hy << n) | (ly >> (32 - n));
            ly <<= n;
        }
        else
        {
            hy = ly << (n - 32);
            ly = 0;
        }
    }

    n = ix - iy;
    while (n--)
    {
        hz = hx - hy;
        lz = lx - ly;

        if (lx < ly)
            hz -= 1;
        if (hz < 0)
        {
            hx = hx + hx + (lx >> 31);
            lx = lx + lx;
        }
        else
        {
            if ((hz | lz) == 0)
                return zero[(unsigned)sx >> 31];
            hx = hz + hz + (lz >> 31);
            lx = lz + lz;
        }
    }

    hz = hx - hy;
    lz = lx - ly;

    if (lx < ly)
        hz -= 1;
    if (hz >= 0)
    {
        hx = hz;
        lx = lz;
    }

    if ((hx | lx) == 0)
        return zero[(unsigned)sx >> 31];
    while (hx < 0x00100000)
    {
        hx = hx + hx + (lx >> 31);
        lx = lx + lx;
        iy -= 1;
    }

    if (iy >= -1022)
    {
        hx = ((hx - 0x00100000) | ((iy + 1023) << 20));
        HIGH(x) = hx | sx;
        LOW(x) = lx;
    }
    else
    {
        n = -1022 - iy;
        if (n <= 20)
        {
            lx = (lx >> n) | ((unsigned)hx << (32 - n));
            hx >>= n;
        }
        else if (n <= 31)
        {
            lx = (hx << (32 - n)) | (lx >> n);
            hx = sx;
        }
        else
        {
            lx = hx >> (n - 32);
            hx = sx;
        }

        HIGH(x) = hx | sx;
        LOW(x) = lx;
        x *= one;
    }

    return x;
}