#include "erl_nif.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifdef __cplusplus
#define BEGIN_C extern "C" {
#define END_C }
#else
#define BEGIN_C
#define ENC_C
#endif



BEGIN_C

static ERL_NIF_TERM mat4(ErlNifEnv *env, int args, const ERL_NIF_TERM argv[])
{
				glm::mat4 d = glm::mat4(1.0f);
				ERL_NIF_TERM o[16];

				const float *pSource = (const float *)glm::value_ptr(d);
				for(int i =0; i<16; ++i) 
								o[i] = enif_make_double(env,(double)pSource[i]);
				return enif_make_tuple_from_array(env,o,16);
}

static ERL_NIF_TERM lookat(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
			int arity;
			const ERL_NIF_TERM *eye,*center,*up;
			ERL_NIF_TERM d[16];

			if(!enif_get_tuple(env,argv[0],&arity,&eye)) {
							return enif_make_badarg(env);
			}
			if(arity!=3) {
							return enif_make_badarg(env);
			}

			if(!enif_get_tuple(env,argv[1],&arity,&center)) {
							return enif_make_badarg(env);
			}
			if(arity!=3) {
							return enif_make_badarg(env);
			}

			if(!enif_get_tuple(env,argv[2],&arity,&up)) {
							return enif_make_badarg(env);
			}
			if(arity != 3) {
							return enif_make_badarg(env);
			}

			glm::dvec3 veye;
			if(!(enif_get_double(env,eye[0],&veye.x) && 
					 enif_get_double(env,eye[1],&veye.y) &&	
					 enif_get_double(env,eye[2],&veye.z))) {
							return enif_make_badarg(env);
			}

			glm::dvec3 vcenter;
			if(!(enif_get_double(env,center[0],&vcenter.x) && 
					 enif_get_double(env,center[1],&vcenter.y) &&	
					 enif_get_double(env,center[2],&vcenter.z))) {
							return enif_make_badarg(env);
			}

			glm::dvec3 vup;
			if(!(enif_get_double(env,up[0],&vup.x) && 
					 enif_get_double(env,up[1],&vup.y) &&	
					 enif_get_double(env,up[2],&vup.z))) {
							return enif_make_badarg(env);
			}

			glm::dmat4 m = glm::lookAt(veye,vcenter,vup);
			const double *pSource = (const double*)glm::value_ptr(m);
			for(int i =0; i<16;++i) 
							d[i] = enif_make_double(env,pSource[i]);
			return enif_make_tuple_from_array(env,d,16);
}



static ERL_NIF_TERM perspective(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
			double fovy,aspect,zNear,zFar;
			ERL_NIF_TERM d[16];

			if(!enif_get_double(env,argv[0], &fovy)) {
							return enif_make_badarg(env);
			}
			if(!enif_get_double(env,argv[1], &aspect)) {
							return enif_make_badarg(env);
			}
			if(!enif_get_double(env,argv[2], &zNear)) {
							return enif_make_badarg(env);
			}
			if(!enif_get_double(env,argv[3], &zFar)) {
							return enif_make_badarg(env);
			}

			glm::dmat4 p = glm::perspective(fovy, aspect,zNear, zFar);
			const double *pSource = (const double*)glm::value_ptr(p);
			for( int i=0; i< 16; ++i) 
							d[i] = enif_make_double(env,pSource[i]);

			return enif_make_tuple_from_array(env, d, 16);
}

static ERL_NIF_TERM rotate(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
				double angle;
				int arity;
				const ERL_NIF_TERM *t,*v;
				double dm[16]; 
				ERL_NIF_TERM d[16];

				if(!enif_get_tuple(env,argv[0],&arity,&t)) {
								return enif_make_badarg(env);
				}
				if(arity!=16) {
								return enif_make_badarg(env);
				}

				for(int i=0; i<16; ++i) {
								enif_get_double(env,t[i],&dm[i]);
				}

				glm::dmat4 matrix = glm::make_mat4(dm);

				if(!enif_get_double(env,argv[1],&angle)) {
								return enif_make_badarg(env);
				}

				if(!enif_get_tuple(env,argv[2],&arity,&v)) {
								return enif_make_badarg(env);
				}
				if(arity != 3) {
								return enif_make_badarg(env);
				}

				glm::dvec3 axis;

			  if(!(enif_get_double(env,v[0],&axis.x) && enif_get_double(env,v[1],&axis.y) &&	enif_get_double(env,v[2],&axis.z))) {
								return enif_make_badarg(env);
				}

				glm::dmat4 rotation = glm::rotate(matrix,angle,axis);
				
				const double *pSource = (const double*)glm::value_ptr(rotation);
				for(int i =0; i<16; ++i) {
							d[i] = enif_make_double(env,pSource[i]);
				}
				return enif_make_tuple_from_array(env,d,16);
}

static ERL_NIF_TERM translate(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
			double x,y,z;
			ERL_NIF_TERM d[16];

			if(!enif_get_double(env,argv[0],&x)) {
							return enif_make_badarg(env);
			}
			if(!enif_get_double(env,argv[1],&y)) {
							return enif_make_badarg(env);
			}
			if(!enif_get_double(env,argv[2],&z)) {
							return enif_make_badarg(env);
			}

			glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3((float)x,(float)y,(float)z));
			const float *pSource = (const float*)glm::value_ptr(trans);
			for(int i =0; i<16;++i) 
							d[i] = enif_make_double(env,(double)pSource[i]);
			return enif_make_tuple_from_array(env,d,16);
}


static ErlNifFunc nif_funcs[] =
{
				{"perspective", 4, perspective},
				{"lookat",3,lookat},
				{"mat4",0,mat4},
				{"translate",3,translate},
				{"rotate",3,rotate}
};

ERL_NIF_INIT(erlglm, nif_funcs, NULL, NULL, NULL,NULL)

END_C

