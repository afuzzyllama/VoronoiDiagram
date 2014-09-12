// Copyright 2014 afuzzyllama. All Rights Reserved.

#include "VoronoiDiagramPrivatePCH.h"
#include "VoronoiDiagramEdge.h"

TSharedPtr<FVoronoiDiagramEdge> FVoronoiDiagramEdge::DELETED(new FVoronoiDiagramEdge());

TSharedPtr<FVoronoiDiagramEdge> FVoronoiDiagramEdge::Bisect(TSharedPtr<FVoronoiDiagramSite> SiteA, TSharedPtr<FVoronoiDiagramSite> SiteB)
{
    float dx, dy;
    TSharedPtr<FVoronoiDiagramEdge> NewEdge(new FVoronoiDiagramEdge());

    NewEdge->LeftSite = SiteA;
    NewEdge->RightSite = SiteB;
    NewEdge->LeftEndPoint = nullptr;
    NewEdge->RightEndPoint = nullptr;
    
    dx = SiteB->GetCoordinate().X - SiteA->GetCoordinate().X;
    dy = SiteB->GetCoordinate().Y - SiteA->GetCoordinate().Y;
    
    NewEdge->c = SiteA->GetCoordinate().X * dx + SiteA->GetCoordinate().Y * dy + (dx * dx + dy * dy) * 0.5f;
    if(FMath::Abs(dx) > FMath::Abs(dy))
    {
        NewEdge->a = 1.0f;
        NewEdge->b = dy/dx;
        NewEdge->c /= dx;
    }
    else
    {
        NewEdge->b = 1.0f;
        NewEdge->a = dx/dy;
        NewEdge->c /= dy;
    }

    return NewEdge;
}

void FVoronoiDiagramEdge::SetEndpoint(TSharedPtr<FVoronoiDiagramVertex> Vertex, EVoronoiDiagramEdge::Type EdgeType)
{
    if(EdgeType == EVoronoiDiagramEdge::Left)
    {
        LeftEndPoint = Vertex;
    }
    else
    {
        RightEndPoint = Vertex;
    }
}

void FVoronoiDiagramEdge::GenerateClippedEndPoints(FIntRect Bounds)
{
    FVector2D MinimumValues(0, 0);
    FVector2D MaximumValues(Bounds.Width(), Bounds.Height());

    TSharedPtr<FVoronoiDiagramVertex> VertexA, VertexB;
    FVector2D PointA, PointB;

    if (a == 1.0f && b >= 0.0f)
    {
        VertexA = RightEndPoint;
        VertexB = LeftEndPoint;
    }
    else 
    {
        VertexA = LeftEndPoint;
        VertexB = RightEndPoint;
    }

    if (a == 1.0f)
    {
        PointA.Y = MinimumValues.Y;
        
        if (VertexA.IsValid() && VertexA->GetCoordinate().Y > MinimumValues.Y)
        {
             PointA.Y = VertexA->GetCoordinate().Y;
        }
        
        if (PointA.Y > MaximumValues.Y)
        {
            return;
        }
        
        PointA.X = c - b * PointA.Y;
        
        PointB.Y = MaximumValues.Y;
        if (VertexB.IsValid() && VertexB->GetCoordinate().Y < MaximumValues.Y)
        {
            PointB.Y = VertexB->GetCoordinate().Y;
        }
        
        if (PointB.Y < MinimumValues.Y)
        {
            return;
        }
        
        PointB.X = c - b * PointB.Y;
        
        if ((PointA.X > MaximumValues.X && PointB.X > MaximumValues.X) || (PointA.X < MinimumValues.X && PointB.X < MinimumValues.X))
        {
            return;
        }
        
        if (PointA.X > MaximumValues.X)
        {
            PointA.X = MaximumValues.X;
            PointA.Y = (c - PointA.X) / b;
        }
        else if (PointA.X < MinimumValues.X)
        {
            PointA.X = MinimumValues.X;
            PointA.Y = (c - PointA.X) / b;
        }
        
        if (PointB.X > MaximumValues.X)
        {
            PointB.X = MaximumValues.X;
            PointB.Y = (c - PointB.X) / b;
        }
        else if (PointB.X < MinimumValues.X)
        {
            PointB.X = MinimumValues.X;
            PointB.Y = (c - PointB.X) / b;
        }
    }
    else
    {
        PointA.X = MinimumValues.X;
        
        if (VertexA.IsValid() && VertexA->GetCoordinate().Y > MinimumValues.X)
        {
            PointA.X = VertexA->GetCoordinate().Y;
        }
        
        if (PointA.X > MaximumValues.X)
        {
            return;
        }
        
        PointA.Y = c - a * PointA.X;
        
        PointB.X = MaximumValues.X;
        
        if (VertexB.IsValid() && VertexB->GetCoordinate().Y < MaximumValues.X)
        {
            PointB.X = VertexB->GetCoordinate().Y;
        }
        
        if (PointB.X < MinimumValues.X)
        {
            return;
        }
        
        PointB.Y = c - a * PointB.X;
        
        if ((PointA.Y > MaximumValues.Y && PointB.Y > MaximumValues.Y) || (PointA.Y < MinimumValues.Y && PointB.Y < MinimumValues.Y))
        {
            return;
        }
        
        if (PointA.Y > MaximumValues.Y)
        {
            PointA.Y = MaximumValues.Y;
            PointA.X = (c - PointA.Y)/a;
        }
        else if (PointA.Y < MinimumValues.Y)
        {
            PointA.Y = MinimumValues.Y;
            PointA.X = (c - PointA.Y)/a;
        }
        
        if (PointB.Y > MaximumValues.Y)
        {
            PointB.Y = MaximumValues.Y;
            PointB.X = (c - PointB.Y)/a;
        }
        else if (PointB.Y < MinimumValues.Y)
        {
            PointB.Y = MinimumValues.Y;
            PointB.X = (c - PointB.Y)/a;
        }
    }

    if (VertexA == LeftEndPoint)
    {
        LeftClippedEndPoint = FVector2D(PointA.X, PointA.Y);
        RightClippedEndPoint = FVector2D(PointB.X, PointB.Y);
    }
    else
    {
        RightClippedEndPoint = FVector2D(PointA.X, PointA.Y);
        LeftClippedEndPoint = FVector2D(PointB.X, PointB.Y);
    }
}

FVoronoiDiagramEdge::FVoronoiDiagramEdge()
: Index(-1)
, a(0.0f)
, b(0.0f)
, c(0.0f)
, LeftEndPoint(nullptr)
, RightEndPoint(nullptr)
, LeftSite(nullptr)
, RightSite(nullptr)
{}