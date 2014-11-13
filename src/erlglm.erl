-module(erlglm).
-author("wbew@mac.com").

-export([perspective/4, lookat/3, mat4/0,translate/3,rotate/3]).

-type erlglm_vector() :: {float(),float(),float()}.
-type erlglm_matrix4() :: {float(),float(),float(),float(),
                           float(),float(),float(),float(),
                           float(),float(),float(),float(),
                           float(),float(),float(),float()}.

-on_load(load_nif/0).
-define(nif_stub, nif_stub_error(?LINE)).
nif_stub_error(Line) ->
        erlang:nif_error({nif_not_loaded,module,?MODULE,line,Line}).


load_nif() ->
        PrivDir = case code:priv_dir(?MODULE) of
                {error, bad_name} ->
                        EbinDir = filename:dirname(code:which(?MODULE)),
                        AppPath = filename:dirname(EbinDir),
                        filename:join(AppPath,"priv");
                Path ->
                        Path
        end,
        erlang:load_nif(filename:join(PrivDir,"erlglm",0)).

-spec perspective(float(), float(), float(), float()) -> erlglm_matrix4().
perspective(_Fovy, _Aspect, _Znear, _Zfar) ->
        ?nif_stub.

-spec lookat(erlglm_vector(), erlglm_vector(), erlglm_vector()) -> erlglm_matrix4().
lookat(_Eye, _Center, _Up) ->
        ?nif_stub.

-spec mat4() -> erlglm_matrix4().
mat4() ->
        ?nif_stub.

-spec translate(float(), float(), float()) -> erlglm_matrix4().
translate(_X,_Y,_Z) ->
        ?nif_stub.

-spec rotate(erlglm_matrix4(), float(), erlglm_vector()) -> erlglm_matrix4().
rotate(_Matrix,_Angle,_Axis) ->
        ?nif_stub.
