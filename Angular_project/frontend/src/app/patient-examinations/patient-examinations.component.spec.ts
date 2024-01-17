import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PatientExaminationsComponent } from './patient-examinations.component';

describe('PatientExaminationsComponent', () => {
  let component: PatientExaminationsComponent;
  let fixture: ComponentFixture<PatientExaminationsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ PatientExaminationsComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(PatientExaminationsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
