#pragma once

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,24.f,FColor::Red,false,-1.f,0,1.f);
#define DRAW_LINE(Location,EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(),Location,EndLocation,FColor::Red,false,-1.f,0,1.f);
#define DRAW_POINT(Location,EndLocation) if(GetWorld()) DrawDebugPoint(GetWorld(),Location + EndLocation, 25.f, FColor::Blue,false,-1.f);
#define Draw_Vector(Location,EndLocation)\
{\
if (GetWorld())\
{\
DrawDebugLine(GetWorld(),Location,Location + EndLocation,FColor::Red,false,-1.f,0,1.f); \
DrawDebugPoint(GetWorld(),Location + EndLocation, 25.f, FColor::Blue,false,-1.f); \
}\
}