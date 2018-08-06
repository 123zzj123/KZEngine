#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include"../KZMath/vector.h"

namespace KZEngine {
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
		//��ɫ���캯��
		explicit ColorTyp(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) :r_(r), g_(g), b_(b), a_(a) {};
		//��ɫ�ӷ�
		ColorTyp operator+ (const ColorTyp& c) const;
		//��ɫ�ӷ�
		void operator+= (const ColorTyp& c);
		//��ɫ�˷�
		ColorTyp operator* (const ColorTyp& c) const;
		//��ɫ����ֵ�˷�
		ColorTyp operator* (float num) const;
		//��ɫ����ֵ�˷�
		void operator*= (float num);
		//������ɫ
		inline void Set(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) { 
			r_ = r;
			g_ = g;
			b_ = b;
			a_ = a;
		}
	}Color, *ColorPtr;

	//��Դ����:ƽ�й⣬��⣬�򵥾۹�ƣ����Ӿ۹�ƣ�������
	enum LightType
	{
		DIRECTION,
		POINT,
		SSPOT,
		CSPOT,
		AMBIENT,
	};

	//��Դ����
	class LightBase
	{
	public:
		//���캯��
		LightBase(const KZEngine::Color& color) :light_colr_(color) {}
		//��ȡ��������
		LightType GetType() { return type_; }
		//��ȡ����ǿ��
		virtual Color CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos = KZMath::KZVector4D(), const KZMath::KZVector4D& vertex_normal = KZMath::KZVector4D());
		//��ȡ��Դ����
		virtual KZMath::KZVector4D GetLightVec(const KZMath::KZVector4D& vertex_pos = KZMath::KZVector4D());
	protected:
		LightType type_;
		Color light_colr_;
	};

	//ƽ�й�
	class DirectionLight : public LightBase
	{
	public:
		//ƽ�й⹹�캯��
		DirectionLight(const Color& color, const KZMath::KZVector4D& dir) :LightBase(color), dir_(dir) { type_ = LightType::DIRECTION; }
		//�������ǿ��
		Color CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos = KZMath::KZVector4D(), const KZMath::KZVector4D& vertex_normal = KZMath::KZVector4D());
		//��ȡ��Դ����
		KZMath::KZVector4D GetLightVec(const KZMath::KZVector4D& vertex_pos);
	private:
		//��Դ����
		KZMath::KZVector4D dir_;
	};

	//���Դ
	class PointLight : public LightBase
	{
	public:
		//���Դ���캯��
		PointLight(const Color& color, const KZMath::KZVector4D& pos, float kc, float kl, float kq) :LightBase(color), pos_(pos), kc_(kc), kl_(kl), kq_(kq) { type_ = LightType::POINT; }
		//�����Դǿ��
		Color CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos = KZMath::KZVector4D(), const KZMath::KZVector4D& vertex_normal = KZMath::KZVector4D());
		//��ȡ��Դ����
		KZMath::KZVector4D GetLightVec(const KZMath::KZVector4D& vertex_pos);
		//��ȡ��Դλ��
		inline KZMath::KZVector4D GetLightPos() { return pos_; };
	private:
		//��Դλ��
		KZMath::KZVector4D pos_;
		//����˥������
		float kc_;
		//����˥������
		float kl_;
		//����˥������
		float kq_;
		//˥������
		float dist_;
	};

	//�򵥾۹��
	class SimpleSpotLight : public LightBase
	{
	public:
		//�򵥾۹�ƹ��캯��
		SimpleSpotLight(const Color& color, const KZMath::KZVector4D& pos, const KZMath::KZVector4D& dir, float kc, float kl, float kq, float pf) :
			LightBase(color), pos_(pos), dir_(dir), kc_(kc), kl_(kl), kq_(kq){ type_ = LightType::SSPOT; }
		//�����Դǿ��
		Color CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos = KZMath::KZVector4D(), const KZMath::KZVector4D& vertex_normal = KZMath::KZVector4D());
		//��ȡ��Դ����
		KZMath::KZVector4D GetLightVec(const KZMath::KZVector4D& vertex_pos);
		//��ȡ��Դλ��
		inline KZMath::KZVector4D GetLightPos() { return pos_; };
	private:
		//��Դλ��
		KZMath::KZVector4D pos_;
		//��Դ����
		KZMath::KZVector4D dir_;
		//����˥������
		float kc_;
		//����˥������
		float kl_;
		//����˥������
		float kq_;
		//�۹��ָ������Ĭ��Ϊ1
		//float pf_;
	};

	//���Ӿ۹��
	class ComplexSpotLight : public LightBase
	{
	public:
		//���Ӿ۹�ƹ��캯��
		ComplexSpotLight(const Color& color, const KZMath::KZVector4D& pos, const KZMath::KZVector4D& dir, float spot_inner_angle, float spot_outter_angle, float kc, float kl, float kq, float pf) :
			LightBase(color), pos_(pos), dir_(dir), spot_inner_angle_(spot_inner_angle), spot_outter_angle_(spot_outter_angle), kc_(kc), kl_(kl), kq_(kq), pf_(pf) { type_ = LightType::CSPOT; }
		//�����Դǿ��
		Color CalculateLightIntensity(const KZMath::KZVector4D& vertex_pos = KZMath::KZVector4D(), const KZMath::KZVector4D& vertex_normal = KZMath::KZVector4D());
		//��ȡ��Դ����
		KZMath::KZVector4D GetLightVec(const KZMath::KZVector4D& vertex_pos);
		//��ȡ��Դλ��
		inline KZMath::KZVector4D GetLightPos() { return pos_; };
	private:
		//��Դλ��
		KZMath::KZVector4D pos_;
		//��Դ����
		KZMath::KZVector4D dir_;
		//�۹����׶��
		float spot_inner_angle_;
		//�۹����׶��
		float spot_outter_angle_;
		//����˥������
		float kc_;
		//����˥������
		float kl_;
		//����˥������
		float kq_;
		//�۹��ָ������
		float pf_;
	};

	//������
	class AmbientLight : public LightBase
	{
	public:
		//�����⹹�캯��
		AmbientLight(const Color& color) :LightBase(color) { type_ = LightType::AMBIENT; }
	private:

	};
}

#endif // !LIGHT_H
