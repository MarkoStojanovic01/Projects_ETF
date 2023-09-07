import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PatientDocProfileComponent } from './patient-doc-profile.component';

describe('PatientDocProfileComponent', () => {
  let component: PatientDocProfileComponent;
  let fixture: ComponentFixture<PatientDocProfileComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ PatientDocProfileComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(PatientDocProfileComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
