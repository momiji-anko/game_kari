#pragma once
#include<SimpleMath.h>
#include<vector>

class PolygonToLineSegmentCollision
{
public:
	/// <summary>
	/// 線分と板ポリゴンの当たり判定
	/// </summary>
	/// <param name="vertices">頂点座標</param>
	/// <param name="linePos">線分の両端座標</param>
	/// <param name="normalVector">法線ベクトルの参照</param>
	/// <returns>true=当たっている　false=当っていない</returns>
	static bool DetectCollision(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const std::vector<DirectX::SimpleMath::Vector3>& linePos, DirectX::SimpleMath::Vector3& normalVector, DirectX::SimpleMath::Vector3& actorPos)
	{
		//各頂点座標
		DirectX::SimpleMath::Vector3 vertex0 = vertices[0];
		DirectX::SimpleMath::Vector3 vertex1 = vertices[1];
		DirectX::SimpleMath::Vector3 vertex2 = vertices[2];

		//線分の両端座標
		DirectX::SimpleMath::Vector3 startLine = linePos[0];
		DirectX::SimpleMath::Vector3 endLine = linePos[1];

		DirectX::SimpleMath::Vector3 lineVec = endLine - startLine;

		//法線ベクトル算出のためのデータ
		//頂点０から頂点1のベクトル
		DirectX::SimpleMath::Vector3 v1 = vertex1 - vertex0;
		//頂点1から頂点2のベクトル
		DirectX::SimpleMath::Vector3 v2 = vertex2 - vertex1;

		//法線ベクトル
		DirectX::SimpleMath::Vector3 normal;
		//法線ベクトルの算出
		normal = v1.Cross(v2);
		DirectX::SimpleMath::Vector3 polygonPosAverage = (vertex0 + vertex1 + vertex2) / 3;

		//ポリゴンの中心点から線分の両端へのベクトルを計算
		DirectX::SimpleMath::Vector3 polygonPosAverageToStartLine = startLine - polygonPosAverage;
		DirectX::SimpleMath::Vector3 polygonPosAverageToEndLine = endLine - polygonPosAverage;

		///線分を平面の衝突判定========================================================================

		//ポリゴンの中心点から線分の両端へのベクトルと法線ベクトルで内積を算出する
		float dot1 = polygonPosAverageToStartLine.Dot(normal);
		float dot2 = polygonPosAverageToEndLine.Dot(normal);
		//計算した内積を掛け算し、０以下であれば当たっている
		if (dot1 * dot2 <= 0)
		{

			//ポリゴンから線分の両端の距離の計算
			float d1 = std::abs(polygonPosAverageToStartLine.Dot(normal));
			float d2 = std::abs(polygonPosAverageToEndLine.Dot(normal));
			//内分比の計算
			float a = d1 / (d1 + d2);

			//頂点０から線分の両端のベクトル
			DirectX::SimpleMath::Vector3 vertex0TostartLine = startLine - vertex0;
			DirectX::SimpleMath::Vector3 vertex0ToendLine = endLine - vertex0;
			//頂点０から線分の内分点計算
			DirectX::SimpleMath::Vector3 v3 = ((1 - a) * vertex0TostartLine) + (a * vertex0ToendLine);
			//内分点の座標計算
			DirectX::SimpleMath::Vector3 v3Pos = vertex0 + v3;

			//ポリゴン上に点があるか判定する

			//V3Posから各頂点の座標へのベクトル作成
			DirectX::SimpleMath::Vector3 vertex0ToV3Pos = v3;
			DirectX::SimpleMath::Vector3 vertex1ToV3Pos = v3Pos - vertex1;
			DirectX::SimpleMath::Vector3 vertex2ToV3Pos = v3Pos - vertex2;

			//左回りになるようにベクトルを作成する
			//頂点０から頂点１へのベクトル
			DirectX::SimpleMath::Vector3 vertex0ToVertex1 = vertex1 - vertex0;
			//頂点１から頂点２へのベクトル
			DirectX::SimpleMath::Vector3 vertex1ToVertex2 = vertex2 - vertex1;
			//頂点２から頂点０へのベクトル
			DirectX::SimpleMath::Vector3 vertex2ToVertex0 = vertex0 - vertex2;

			//V3Posから０頂点の座標へのベクトルと頂点０から頂点１へのベクトルの外積算出
			DirectX::SimpleMath::Vector3 cross1 = vertex0ToV3Pos.Cross(vertex0ToVertex1);
			//V3Posから１頂点の座標へのベクトルと頂点１から頂点２へのベクトルの外積算出
			DirectX::SimpleMath::Vector3 cross2 = vertex1ToV3Pos.Cross(vertex1ToVertex2);
			//V3Posから２頂点の座標へのベクトルと頂点２から頂点０へのベクトルの外積算出
			DirectX::SimpleMath::Vector3 cross3 = vertex2ToV3Pos.Cross(vertex2ToVertex0);

			//算出した外積同士を内積で計算
			float checkDot1 = cross1.Dot(cross2);
			float checkDot2 = cross2.Dot(cross3);
			float checkDot3 = cross3.Dot(cross1);

			float md = lineVec.Dot(normal);

			//算出した内積を掛け算し０以上であればポリゴンの中に線分がある
			if ((checkDot1 * checkDot2 >= 0 && checkDot2 * checkDot3 >= 0 && checkDot3 * checkDot1 >= 0)&& md > 1.0)
			{
				normalVector = normal;
				//actorPos = ActorPolygonPenetration(endLine, vertices, normal);
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// カメラ線分と板ポリゴンの当たり判定
	/// </summary>
	/// <param name="vertices">頂点座標</param>
	/// <param name="linePos">線分の両端座標</param>
	/// <param name="normalVector">法線ベクトルの参照</param>
	/// <returns>true=当たっている　false=当っていない</returns>
	static bool DetectCollision_Camera(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const std::vector<DirectX::SimpleMath::Vector3>& linePos, DirectX::SimpleMath::Vector3& normalVector, DirectX::SimpleMath::Vector3& actorPos)
	{
		//各頂点座標
		DirectX::SimpleMath::Vector3 vertex0 = vertices[0];
		DirectX::SimpleMath::Vector3 vertex1 = vertices[1];
		DirectX::SimpleMath::Vector3 vertex2 = vertices[2];

		//線分の両端座標
		DirectX::SimpleMath::Vector3 startLine = linePos[0];
		DirectX::SimpleMath::Vector3 endLine = linePos[1];

		DirectX::SimpleMath::Vector3 lineVec = endLine - startLine;

		//法線ベクトル算出のためのデータ
		//頂点０から頂点1のベクトル
		DirectX::SimpleMath::Vector3 v1 = vertex1 - vertex0;
		//頂点1から頂点2のベクトル
		DirectX::SimpleMath::Vector3 v2 = vertex2 - vertex1;

		//法線ベクトル
		DirectX::SimpleMath::Vector3 normal;
		//法線ベクトルの算出
		normal = v1.Cross(v2);
		DirectX::SimpleMath::Vector3 polygonPosAverage = (vertex0 + vertex1 + vertex2) / 3;

		//ポリゴンの中心点から線分の両端へのベクトルを計算
		DirectX::SimpleMath::Vector3 polygonPosAverageToStartLine = startLine - polygonPosAverage;
		DirectX::SimpleMath::Vector3 polygonPosAverageToEndLine = endLine - polygonPosAverage;

		///線分を平面の衝突判定========================================================================

		//ポリゴンの中心点から線分の両端へのベクトルと法線ベクトルで内積を算出する
		float dot1 = polygonPosAverageToStartLine.Dot(normal);
		float dot2 = polygonPosAverageToEndLine.Dot(normal);
		//計算した内積を掛け算し、０以下であれば当たっている
		if (dot1 * dot2 <= 0)
		{

			//ポリゴンから線分の両端の距離の計算
			float d1 = std::abs(polygonPosAverageToStartLine.Dot(normal));
			float d2 = std::abs(polygonPosAverageToEndLine.Dot(normal));
			//内分比の計算
			float a = d1 / (d1 + d2);

			//頂点０から線分の両端のベクトル
			DirectX::SimpleMath::Vector3 vertex0TostartLine = startLine - vertex0;
			DirectX::SimpleMath::Vector3 vertex0ToendLine = endLine - vertex0;
			//頂点０から線分の内分点計算
			DirectX::SimpleMath::Vector3 v3 = ((1 - a) * vertex0TostartLine) + (a * vertex0ToendLine);
			//内分点の座標計算
			DirectX::SimpleMath::Vector3 v3Pos = vertex0 + v3;

			//ポリゴン上に点があるか判定する

			//V3Posから各頂点の座標へのベクトル作成
			DirectX::SimpleMath::Vector3 vertex0ToV3Pos = v3;
			DirectX::SimpleMath::Vector3 vertex1ToV3Pos = v3Pos - vertex1;
			DirectX::SimpleMath::Vector3 vertex2ToV3Pos = v3Pos - vertex2;

			//左回りになるようにベクトルを作成する
			//頂点０から頂点１へのベクトル
			DirectX::SimpleMath::Vector3 vertex0ToVertex1 = vertex1 - vertex0;
			//頂点１から頂点２へのベクトル
			DirectX::SimpleMath::Vector3 vertex1ToVertex2 = vertex2 - vertex1;
			//頂点２から頂点０へのベクトル
			DirectX::SimpleMath::Vector3 vertex2ToVertex0 = vertex0 - vertex2;

			//V3Posから０頂点の座標へのベクトルと頂点０から頂点１へのベクトルの外積算出
			DirectX::SimpleMath::Vector3 cross1 = vertex0ToV3Pos.Cross(vertex0ToVertex1);
			//V3Posから１頂点の座標へのベクトルと頂点１から頂点２へのベクトルの外積算出
			DirectX::SimpleMath::Vector3 cross2 = vertex1ToV3Pos.Cross(vertex1ToVertex2);
			//V3Posから２頂点の座標へのベクトルと頂点２から頂点０へのベクトルの外積算出
			DirectX::SimpleMath::Vector3 cross3 = vertex2ToV3Pos.Cross(vertex2ToVertex0);

			//算出した外積同士を内積で計算
			float checkDot1 = cross1.Dot(cross2);
			float checkDot2 = cross2.Dot(cross3);
			float checkDot3 = cross3.Dot(cross1);

			//算出した内積を掛け算し０以上であればポリゴンの中に線分がある
			if ((checkDot1 * checkDot2 >= 0 && checkDot2 * checkDot3 >= 0 && checkDot3 * checkDot1 >= 0)&& lineVec.Dot(normal) > 0.5)
			{
				normalVector = normal;
				actorPos = v3Pos;
				return true;
			}
		}
		return false;
	}



	/// <summary>
	/// アクターのスライドベクトルを計算する
	/// </summary>
	/// <param name="normalVec">ポリゴンの法線ベクトル</param>
	/// <param name="actorVel">アクターの移動量</param>
	/// <returns>スライドベクトル</returns>
	static DirectX::SimpleMath::Vector3 SlideVecCalculation(const DirectX::SimpleMath::Vector3& normalVec, const DirectX::SimpleMath::Vector3& actorVel)
	{
		DirectX::SimpleMath::Vector3 normalVector = normalVec;
		//法線ベクトルの正規化
		normalVector.Normalize();
 
		if (std::abs(normalVector.y) >= 0.7)
			return DirectX::SimpleMath::Vector3::Zero;

		//プレイヤーの移動ベクトルと法線ベクトルの内積の計算
		float dot = actorVel.Dot(normalVector);
		//スライドする量計算
		DirectX::SimpleMath::Vector3 slideAmount = { actorVel.x - dot,actorVel.y - dot,actorVel.z - dot };
		//スライドベクトルを計算
		DirectX::SimpleMath::Vector3 slideVec = normalVector * slideAmount;
		//スライドベクトルを返す
		return slideVec;
	}

	/// <summary>
	/// ステージのポリゴンとアクターのめり込み処理
	/// </summary>
	/// <param name="actor">アクター/param>
	/// <param name="polygonVertexPos">ポリゴンの頂点座標</param>
	/// <param name="normalVec">法線ベクトル</param>
	static DirectX::SimpleMath::Vector3 ActorPolygonPenetration(const DirectX::SimpleMath::Vector3& actor, const std::vector<DirectX::SimpleMath::Vector3>& polygonVertexPos, const DirectX::SimpleMath::Vector3& normalVec)
	{
		//アクターを少しめり込ませる用変数
		float actorPenetration = 0.1f;

		//めり込み処理
		//アクターの座標取得
		DirectX::SimpleMath::Vector3 actorPos = actor;
		//アクターの立つY座標計算
		actorPos.y = polygonVertexPos[0].y + (-normalVec.x * (actorPos.x - polygonVertexPos[0].x) - normalVec.z * (actorPos.z - polygonVertexPos[0].z)) / normalVec.y;
		//アクターがガクガクするので少しだけ下にめり込ませる
		actorPos.y -= actorPenetration;
		
		return actorPos;
	}

};