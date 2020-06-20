#ifndef TENACITAS_TRANSLATIONS_TRANSLATIONS_H
#define TENACITAS_TRANSLATIONS_TRANSLATIONS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas@gmail.com
/// \date 06/2020

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <utility>

#include <status/result.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace status {

/// \brief text associated to the result
typedef std::string text;

/// \brief \p code associated to a \p text
typedef std::pair<code, text> term;

/// \brief collection of \p term
typedef std::set<term> terms;

/// \brief collection of owner a
typedef std::pair<owner, std::set<term>> owner_terms;

/// \brief defines the possible languages identifiers
struct language {
  language() = delete;
  language(const language &) = delete;
  language(language &&) = delete;
  language &operator=(const language &) = delete;
  language &operator=(language &&) = delete;

  static const language af;
  static const language sq;
  static const language am;
  static const language ar_dz;
  static const language ar_bh;
  static const language ar_eg;
  static const language ar_iq;
  static const language ar_jo;
  static const language ar_kw;
  static const language ar_lb;
  static const language ar_ly;
  static const language ar_ma;
  static const language ar_om;
  static const language ar_qa;
  static const language ar_sa;
  static const language ar_sy;
  static const language ar_tn;
  static const language ar_ae;
  static const language ar_ye;
  static const language hy;
  static const language as;
  static const language az_az;
  static const language eu;
  static const language be;
  static const language bn;
  static const language bs;
  static const language bg;
  static const language my;
  static const language ca;
  static const language zh_cn;
  static const language zh_hk;
  static const language zh_mo;
  static const language zh_sg;
  static const language zh_tw;
  static const language hr;
  static const language cs;
  static const language da;
  static const language dv;
  static const language nl_be;
  static const language nl_nl;
  static const language en_au;
  static const language en_bz;
  static const language en_ca;
  static const language en_cb;
  static const language en_gb;
  static const language en_in;
  static const language en_ie;
  static const language en_jm;
  static const language en_nz;
  static const language en_ph;
  static const language en_za;
  static const language en_tt;
  static const language en_us;
  static const language et;
  static const language mk;
  static const language fo;
  static const language fa;
  static const language fi;
  static const language fr_be;
  static const language fr_ca;
  static const language fr_fr;
  static const language fr_lu;
  static const language fr_ch;
  static const language gd_ie;
  static const language gd;
  static const language de_at;
  static const language de_de;
  static const language de_li;
  static const language de_lu;
  static const language de_ch;
  static const language el;
  static const language gn;
  static const language gu;
  static const language he;
  static const language hi;
  static const language hu;
  static const language is;
  static const language code;
  static const language it_it;
  static const language it_ch;
  static const language ja;
  static const language kn;
  static const language ks;
  static const language kk;
  static const language km;
  static const language ko;
  static const language lo;
  static const language la;
  static const language lv;
  static const language lt;
  static const language ms_bn;
  static const language ms_my;
  static const language ml;
  static const language mt;
  static const language mi;
  static const language mr;
  static const language mn;
  static const language ne;
  static const language no_no;
  static const language pl;
  static const language pt_br;
  static const language pt_pt;
  static const language pa;
  static const language rm;
  static const language ro_mo;
  static const language ro;
  static const language ru;
  static const language ru_mo;
  static const language sa;
  static const language sr_sp;
  static const language tn;
  static const language sd;
  static const language si;
  static const language sk;
  static const language sl;
  static const language so;
  static const language sb;
  static const language es_ar;
  static const language es_bo;
  static const language es_cl;
  static const language es_co;
  static const language es_cr;
  static const language es_do;
  static const language es_ec;
  static const language es_sv;
  static const language es_gt;
  static const language es_hn;
  static const language es_mx;
  static const language es_ni;
  static const language es_pa;
  static const language es_py;
  static const language es_pe;
  static const language es_pr;
  static const language es_es;
  static const language es_uy;
  static const language es_ve;
  static const language sw;
  static const language sv_fi;
  static const language sv_se;
  static const language tg;
  static const language ta;
  static const language tt;
  static const language te;
  static const language th;
  static const language bo;
  static const language ts;
  static const language tr;
  static const language tk;
  static const language uk;
  static const language ur;
  static const language uz_uz;
  static const language vi;
  static const language cy;
  static const language xh;
  static const language yi;
  static const language zu;

  bool operator==(const language &p_language) const {
    return m_value == p_language.m_value;
  }

  bool operator!=(const language &p_language) const {
    return m_value != p_language.m_value;
  }

  bool operator<(const language &p_language) const {
    return m_value < p_language.m_value;
  }

  bool operator>(const language &p_language) const {
    return m_value > p_language.m_value;
  }

private:
  language(const std::string &p_value) : m_value(p_value) {}

private:
  const std::string m_value;
};

/// \brief association of a \p language to \p projects of \p terms
typedef std::pair<language, owner_terms> dictionary;

} // namespace status
} // namespace tenacitas

#endif
