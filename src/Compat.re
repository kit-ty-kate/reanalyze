#if OCAML_MINOR >= 8
let getStringTag = s => switch (s) {
  | Format.String_tag(s) => s
  | _ => ""
}
#else
let getStringTag = s => s
#endif

#if OCAML_MINOR >= 8
let filter_map = List.filter_map
#else
/* https://github.com/ocaml/ocaml/blob/9a31c888b177f3aa603bbbe17852cbb57f047df4/stdlib/list.ml#L254-L262 passed though refmt */
let filter_map = f => {
  let rec aux = accu =>
    fun
    | [] => List.rev(accu)
    | [x, ...l] =>
      switch (f(x)) {
      | None => aux(accu, l)
      | Some(v) => aux([v, ...accu], l)
      };

  aux([]);
};
#endif

let getStringValue = const => switch const {
#if OCAML_MINOR >= 11
  | Parsetree.Pconst_string(s, _, _) => s
#else
  | Parsetree.Pconst_string(s, _) => s
#endif
  | _ => assert false
};

let getConstString = const => switch const {
#if OCAML_MINOR >= 11
  | Asttypes.Const_string(s, _, _) => s
#else
  | Asttypes.Const_string(s, _) => s
#endif
  | _ => assert false
};

#if OCAML_MINOR >= 11
type typedtreeCase('a) = Typedtree.case('a)
#else
type typedtreeCase('a) = Typedtree.case
#endif

#if OCAML_MINOR >= 11
type generalPattern('a) = Typedtree.general_pattern('a)
#else
type generalPattern('a) = Typedtree.pattern
#endif

let unboxPatCstrName = (pat) => {
#if OCAML_MINOR >= 11
  switch (pat.Typedtree.pat_desc) {
    | Typedtree.Tpat_value(v) =>
      switch ((v :> Typedtree.pattern_data(Typedtree.pattern_desc(Typedtree.value))).pat_desc) {
        | Tpat_construct(_, {cstr_name}, _) => Some(cstr_name)
        | _ => None
      }
    | _ => None
  }
#else
  switch (pat.Typedtree.pat_desc) {
    | Tpat_construct(_, {cstr_name}, _) => Some(cstr_name)
    | _ => None
  }
#endif
}


#if OCAML_MINOR >= 8
let setOpenCloseTag = (openTag, closeTag) => {
  Format.mark_open_stag: openTag,
  mark_close_stag: closeTag,
  print_open_stag: _ => (),
  print_close_stag: _ => (),
}
#else
let setOpenCloseTag = (openTag, closeTag) => {
  Format.mark_open_tag: openTag,
  mark_close_tag: closeTag,
  print_open_tag: _ => (),
  print_close_tag: _ => (),
}
#endif

let pp_set_formatter_tag_functions =
#if OCAML_MINOR >= 8
    Format.pp_set_formatter_stag_functions;
#else
    Format.pp_set_formatter_tag_functions;
#endif

let getSigValue = si => switch si {
#if OCAML_MINOR >= 8
  | Types.Sig_value(id, {Types.val_loc, val_kind, val_type}, _) =>
    (id, val_loc, val_kind, val_type)
#else
  | Types.Sig_value(id, {Types.val_loc, val_kind, val_type}) =>
    (id, val_loc, val_kind, val_type)
#endif
  | _ => assert false
}

let getSigType = si => switch si {
#if OCAML_MINOR >= 8
  | Types.Sig_type(id, t, _, _) =>
    (id, t)
#else
  | Types.Sig_type(id, t, _) =>
    (id, t)
#endif
  | _ => assert false
}

let getSigModuleModtype = si => switch si {
#if OCAML_MINOR >= 8
  | Types.Sig_module(id, _, {Types.md_type: moduleType, md_loc:loc}, _, _)
  | Types.Sig_modtype(id, {Types.mtd_type: Some(moduleType), mtd_loc:loc}, _) =>
    Some((id, moduleType, loc))
#else
  | Types.Sig_module(id, {Types.md_type: moduleType, md_loc:loc}, _)
  | Types.Sig_modtype(id, {Types.mtd_type: Some(moduleType), mtd_loc:loc}) =>
    Some((id, moduleType, loc))
#endif
  | _ => None
}

let getMtyFunctorModuleType = (moduleType: Types.module_type) => switch moduleType {
#if OCAML_MINOR >= 10
  | Mty_functor(Named(_, mtParam), mt) => Some((Some(mtParam), mt))
  | Mty_functor(Unit, mt) => Some((None, mt))
#else
  | Mty_functor(_, mtParam, mt) => Some((mtParam, mt))
#endif
  | _ => None
}

let getTexpMatch = desc => switch desc {
#if OCAML_MINOR >= 8
  | Typedtree.Texp_match(e, cases, partial) =>
    (e, cases, partial)
#else
  | Typedtree.Texp_match(e, casesOK, casesExn, partial) =>
    (e, casesOK @ casesExn, partial)
#endif
  | _ => assert false
}

let texpMatchGetExceptions = desc => switch desc {
#if OCAML_MINOR >= 8
  | Typedtree.Texp_match(_, cases, _) =>
    cases
    |> List.filter_map(({Typedtree.c_lhs: pat}) =>
          switch (pat.pat_desc) {
          | Tpat_exception({pat_desc}) => Some(pat_desc)
          | _ => None
          })
#else
  | Typedtree.Texp_match(_, _, casesExn, _) =>
    casesExn |> List.map ((case: Typedtree.case) => case.c_lhs.pat_desc)
#endif
  | _ => assert false
}


let texpMatchHasExceptions = desc => texpMatchGetExceptions(desc) != []



let getPayload = x => {
#if OCAML_MINOR >= 8
 let {Parsetree.attr_name: {txt}, attr_payload: payload} = x;
#else
 let ({Asttypes.txt}, payload) = x;
#endif
 (txt, payload)
}

module Ident = {
  include Ident;
#if OCAML_MINOR >= 8
  let create = Ident.create_local
#endif
}

let tstrExceptionGet = (x : Typedtree.structure_item_desc) => switch x {
#if OCAML_MINOR >= 8
  | Tstr_exception({tyexn_constructor: {ext_id}, tyexn_loc}) =>
    Some((ext_id, tyexn_loc))
#else
  | Tstr_exception({ext_id, ext_loc}) =>
    Some((ext_id, ext_loc))
#endif
  | _ => None
};

#if OCAML_MINOR >= 10
let moduleIdName = nameOpt => switch nameOpt {
  | None => "UnnamedModule"
  | Some(name) => name |> Ident.name
};
#else
let moduleIdName = name => name |> Ident.name;
#endif
