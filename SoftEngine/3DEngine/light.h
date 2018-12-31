#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include"../KZMath/vector.h"
#include"../KZMath/matrix.h"

namespace KZEngine {
	typedef struct FixedColorTyp
	{
	public:
		inline void Set(int32_t r = 0, int32_t g = 0, int32_t b = 0, int32_t a = 255) {
			r_ = r;
			g_ = g;
			b_ = b;
			a_ = a;
		}
	public:
		int32_t r_;
		int32_t g_;
		int32_t b_;
		int32_t a_;
	}FixColor, *FixColorPtr;
	typedef struct ColorTyp {
	public:
		struct
		{
			int rgba_;
			unsigned char arr_[4];
			struct
			{
				unsigned char r_, g_, b_, a_;
			};
		};
	public:
		//颜色构造函数
		explicit ColorTyp(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) :r_(r), g_(g), b_(b), a_(a) {};
		//颜色加法
		ColorTyp operator+ (const ColorTyp& c) const;
		//颜色加法
		void operator+= (const ColorTyp& c);
		//颜色乘法
		ColorTyp operator* (const ColorTyp& c) const;
		//颜色乘法
		void operator*= (const ColorTyp& c);
		//颜色浮点值乘法
		ColorTyp operator* (float num) const;
		//颜色浮点值乘法
		void operator*= (float num);
		//设置颜色
		inline void Set(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) { 
			r_ = r;
			g_ = g;
			b_ = b;
			a_ = a;
		}
	}Color, *ColorPtr;

	//光源类型:平行光，点光，简单聚光灯，复杂聚光灯，环境光
	enum LightType
	{
		DIRECTION,
		POINT,
		SSPOT,
		CSPOT,
		AMBIENT,
	};

	//光源基类
	class LightBase
	{
	public:
		//构造函数
		LightBase(const KZEngine::Color& color) :light_colr_(color) {}
		//获取光照类型
		LightType GetType() { return type_; }
		//获取光照强度
		virtual Color CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>());
		//获取光源向量
		virtual KZMath::KZVector4D<float> GetLightVec(const KZMath::KZVector4D<float>& vertex_pos = KZMath::KZVector4D<float>());
		//获取光源位置
		virtual void GetLightPos(KZMath::KZVector4D<float>& light_pos) { return; };
	protected:
		LightType type_;
		Color light_colr_;
	};

	//平行光
	class DirectionLight : public LightBase
	{
	public:
		//平行光构造函数
		DirectionLight(const Color& color, const KZMath::KZVector4D<float>& dir) :LightBase(color), dir_(dir) { type_ = LightType::DIRECTION; }
		//计算光照强度
		Color CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>()) override;
		//获取光源向量
		KZMath::KZVector4D<float> GetLightVec(const KZMath::KZVector4D<float>& vertex_pos) override;
	private:
		//光源方向
		KZMath::KZVector4D<float> dir_;
	};

	//点光源
	class PointLight : public LightBase
	{
	public:
		//点光源构造函数
		PointLight(const Color& color, const KZMath::KZVector4D<float>& pos, float kc, float kl, float kq) :LightBase(color), pos_(pos), kc_(kc), kl_(kl), kq_(kq) { type_ = LightType::POINT; }
		//计算光源强度
		Color CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>());
		//获取光源向量
		KZMath::KZVector4D<float> GetLightVec(const KZMath::KZVector4D<float>& vertex_pos);
		////获取光源位置
		//inline KZMath::KZVector4D<float> GetLightPos() { return pos_; };
		//获取光源位置
		void GetLightPos(KZMath::KZVector4D<float>& light_pos) { light_pos = pos_; return; };
	private:
		//光源位置
		KZMath::KZVector4D<float> pos_;
		//常数衰减因子
		float kc_;
		//线性衰减因子
		float kl_;
		//二次衰减因子
		float kq_;
		//衰减距离
		float dist_;
	};

	//简单聚光灯
	class SimpleSpotLight : public LightBase
	{
	public:
		//简单聚光灯构造函数
		SimpleSpotLight(const Color& color, const KZMath::KZVector4D<float>& pos, const KZMath::KZVector4D<float>& dir, float kc, float kl, float kq, float pf) :
			LightBase(color), pos_(pos), dir_(dir), kc_(kc), kl_(kl), kq_(kq){ type_ = LightType::SSPOT; }
		//计算光源强度
		Color CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>());
		//获取光源向量
		KZMath::KZVector4D<float> GetLightVec(const KZMath::KZVector4D<float>& vertex_pos);
		////获取光源位置
		//inline KZMath::KZVector4D<float> GetLightPos() { return pos_; };
		//获取光源位置
		void GetLightPos(KZMath::KZVector4D<float>& light_pos) { light_pos = pos_; return; };
	private:
		//光源位置
		KZMath::KZVector4D<float> pos_;
		//光源方向
		KZMath::KZVector4D<float> dir_;
		//常数衰减因子
		float kc_;
		//线性衰减因子
		float kl_;
		//二次衰减因子
		float kq_;
		//聚光灯指数因子默认为1
		//float pf_;
	};

	//复杂聚光灯
	class ComplexSpotLight : public LightBase
	{
	public:
		//复杂聚光灯构造函数
		ComplexSpotLight(const Color& color, const KZMath::KZVector4D<float>& pos, const KZMath::KZVector4D<float>& dir, float spot_inner_angle, float spot_outter_angle, float kc, float kl, float kq, float pf) :
			LightBase(color), pos_(pos), dir_(dir), spot_inner_angle_(spot_inner_angle), spot_outter_angle_(spot_outter_angle), kc_(kc), kl_(kl), kq_(kq), pf_(pf) { type_ = LightType::CSPOT; }
		//计算光源强度
		Color CalculateLightIntensity(const KZMath::KZVector4D<float>& vertex_pos = KZMath::KZVector4D<float>(), const KZMath::KZVector4D<float>& vertex_normal = KZMath::KZVector4D<float>());
		//获取光源向量
		KZMath::KZVector4D<float> GetLightVec(const KZMath::KZVector4D<float>& vertex_pos);
		//获取光源位置
		//inline KZMath::KZVector4D<float> GetLightPos() { return pos_; };
		//获取光源位置
		void GetLightPos(KZMath::KZVector4D<float>& light_pos) { light_pos = pos_; return; };
	private:
		//光源位置
		KZMath::KZVector4D<float> pos_;
		//光源方向
		KZMath::KZVector4D<float> dir_;
		//聚光灯内锥角
		float spot_inner_angle_;
		//聚光灯外锥角
		float spot_outter_angle_;
		//常数衰减因子
		float kc_;
		//线性衰减因子
		float kl_;
		//二次衰减因子
		float kq_;
		//聚光灯指数因子
		float pf_;
	};

	//环境光
	class AmbientLight : public LightBase
	{
	public:
		//环境光构造函数
		AmbientLight(const Color& color) :LightBase(color) { type_ = LightType::AMBIENT; }
	private:

	};
}

#endif // !LIGHT_H
