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
    G4LogicalVolume* GetPhantomScoringVolume() const { return phantomScoringVolume; }

protected:
    G4LogicalVolume* scoringVolume;
    G4LogicalVolume *phantomScoringVolume;

private:
    G4Material *FindMaterial(const G4String &name);
    void DefineMaterials();

    G4Material *Al, *Ti, *Cu, *Ta, *W, *Au;
    G4Material *air, *mylar, *water, *kapton, *tungstenAlloy;
};

#endif
