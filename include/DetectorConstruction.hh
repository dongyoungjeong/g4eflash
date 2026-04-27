#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;
    G4LogicalVolume* GetScoringVolume() const { return scoringVolume; }
    
protected:
    G4LogicalVolume* scoringVolume;

private:
    void DefineMaterials();

    G4Material *Al, *Cu, *Ta, *W, *Au;
    G4Material *air, *mylar, *kapton, *tungstenAlloy;
};

#endif
